// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/Dns.h"
#include "Net/Socket.h"
#include "IO/FileReference.h"
#include "Misc/PlatformMisc.h"
#include <map>
#include <set>

#include "Misc/WindowsPlatformMisc.h"

Task<std::vector<IPHostEntry>> Dns::GetHostEntryAsync(String hostname, std::stop_token cancellationToken)
{
	if (hostname == TEXT("localhost"))
	{
		auto entry = IPHostEntry(std::vector{ IPAddress::Loopback(), IPAddress::Loopback(true) }, std::vector<String>{});
		co_return std::vector{ entry };
	}

	if (IPAddress addr; IPAddress::TryParse(hostname, &addr))
	{
		auto entry = IPHostEntry(std::vector{ addr }, std::vector<String>{});
		co_return std::vector{ entry };
	}

	hostname = hostname.ToLower();
	std::map<IPAddress, std::vector<String>> ipMap;

#if PLATFORM_WINDOWS
	String hostpath = PlatformMisc::GetSystemPath() + TEXT("\\drivers\\etc\\hosts");
#endif

	FileReference fr(hostpath);
	String hosts = co_await fr.ReadAllTextAsync(cancellationToken);

	std::vector<String> lines = hosts.Split('\n', EStringSplitOptions::RemoveEmptyEntries | EStringSplitOptions::TrimEntries);
	for (auto& line : lines)
	{
		if (line[0] == '#')
		{
			// is comment.
			continue;
		}

		std::array<char_t, 2> separators = { ' ', '\t' };
		std::vector<String> pair = line.Split(separators, EStringSplitOptions::RemoveEmptyEntries | EStringSplitOptions::TrimEntries);
		if (pair.size() < 2)
		{
			continue;
		}

		for (size_t i = 0; i < pair.size(); ++i)
		{
			String name = pair[i].ToLower();
			if (name == hostname)
			{
				IPAddress addr;
				if (IPAddress::TryParse(pair[0], &addr) == false)
				{
					break;
				}

				std::vector<String>& aliases = ipMap[addr];
				aliases.insert(aliases.end(), pair.begin() + 1, pair.end());
				break;
			}
		}
	}

	// One or more addresses are found.
	if (ipMap.size() > 0)
	{
		std::set<String> aliases;
		std::vector<IPAddress> addresses;
		for (auto& pair : ipMap)
		{
			addresses.emplace_back(pair.first);
			aliases.insert(pair.second.begin(), pair.second.end());
		}

		std::vector<String> aliases_v;
		aliases_v.reserve(aliases.size());
		for (auto& alias : aliases)
		{
			aliases_v.emplace_back(alias);
		}

		co_return{ IPHostEntry(addresses, aliases_v) };
	}

	std::string host = hostname.AsCodepage(65001);

	static IPEndPoint DNSServers[] =
	{
		IPEndPoint(IPAddress(1, 1, 1, 1), 53)
	};

	Socket s(EAddressFamily::InterNetwork, ESocketType::Dgram, EProtocolType::Udp);
	s.Bind(IPEndPoint::Any());

	int pid = _getpid();

	SocketBuffer buf = SocketBuffer::Alloc(2048);
	uint8* ptr = reinterpret_cast<uint8*>(buf.GetBuffer());

	//Set the DNS structure to standard queries
	DNS_HEADER* dns = (DNS_HEADER*)ptr;
	ptr += sizeof(DNS_HEADER);

	dns->id = (unsigned short)htons(pid);
	dns->qr = 0; //This is a query
	dns->opcode = 0; //This is a standard query
	dns->aa = 0; //Not Authoritative
	dns->tc = 0; //This message is not truncated
	dns->rd = 1; //Recursion Desired
	dns->ra = 0; //Recursion not available! hey we dont have it (lol)
	dns->z = 0;
	dns->ad = 0;
	dns->cd = 0;
	dns->rcode = 0;
	dns->q_count = htons(1); //we have only 1 question
	dns->ans_count = 0;
	dns->auth_count = 0;
	dns->add_count = 0;

	//point to the query portion
	uint8* qname = ptr;
	size_t len = _calc_dns_query_name(qname, host);
	ptr += len + 1;

	QUESTION* qinfo = (QUESTION*)ptr; //fill it
	ptr += sizeof(QUESTION);
	qinfo->qtype = htons(T_A); //type of the query , A , MX , CNAME , NS etc
	qinfo->qclass = htons(1); //its internet (lol)

	co_await s.SendToAsync(DNSServers[0], buf, cancellationToken);

	static thread_local IPEndPoint sDummyEP;
	co_await s.ReceiveFromAsync(sDummyEP, buf, cancellationToken);

	ptr = reinterpret_cast<uint8*>(buf.GetBuffer());
	ptr += sizeof(DNS_HEADER);
	ptr += strlen((const char*)qname) + 1;
	ptr += sizeof(QUESTION);
	uint8* reader = ptr;

	static auto ReadName = [](uint8* reader, uint8* buffer, int* count)
	{
		std::string name;
		unsigned int p = 0, jumped = 0, offset;
		int i, j;

		*count = 1;

		//read the names in 3www6google3com format
		while (*reader != 0)
		{
			if (*reader >= 192)
			{
				offset = (*reader) * 256 + *(reader + 1) - 49152; //49152 = 11000000 00000000 ;)
				reader = buffer + offset - 1;
				jumped = 1; //we have jumped to another location so counting wont go up!
			}
			else
			{
				name += (char)*reader;
				++p;
			}

			reader = reader + 1;

			if (jumped == 0)
			{
				*count = *count + 1; //if we havent jumped to another location then we can count up
			}
		}

		if (jumped == 1)
		{
			*count = *count + 1; //number of steps we actually moved forward in the packet
		}

		//now convert 3www6google3com0 to www.google.com
		for (i = 0; i < name.length(); i++)
		{
			p = name[i];
			for (j = 0; j < (int)p; j++)
			{
				name[i] = name[i + 1];
				i = i + 1;
			}
			name[i] = '.';
		}
		name[i - 1] = '\0'; //remove the last dot
		return name;
	};

	size_t ans_count = ntohs(dns->ans_count);
	std::vector<String> aliases;
	std::vector<IPAddress> addresses;
	for (size_t i = 0; i < ans_count; ++i)
	{
		RES_RECORD ans;
		ans.name = _read_dns_query_name(reader, reinterpret_cast<uint8*>(buf.GetBuffer()), &reader);
		ans.resource = (R_DATA*)reader;
		reader += sizeof(R_DATA);

		if (ntohs(ans.resource->type) == 1)
		{
			size_t sz = ntohs(ans.resource->data_len);
			ans.rdata.resize(sz);

			for (size_t j = 0; j < sz; ++j)
			{
				ans.rdata[j] = reader[j];
			}

			ans.rdata[sz] = 0;
			reader += sz;

			addresses.emplace_back(IPAddress(ans.rdata[0], ans.rdata[1], ans.rdata[2], ans.rdata[3]));
		}
		else
		{
			ans.rdata = _read_dns_query_name(reader, reinterpret_cast<uint8*>(buf.GetBuffer()), &reader);
			aliases.emplace_back(String(ans.rdata));
		}
	}

	co_return{ IPHostEntry(addresses, aliases) };
}

size_t Dns::_calc_dns_query_name(uint8* ptr, std::string_view host)
{
	int32 count = 0;
	const char* seek = host.data();
	uint8* const s = ptr;

	auto flush = [&]()
	{
		*ptr++ = (uint8)count;
		memcpy(ptr, seek, count);
		ptr += count;
		seek += count + 1;
		count = 0;
	};

	for (size_t i = 0; i < host.length(); ++i)
	{
		if (host[i] == '.')
		{
			flush();
		}
		else
		{
			++count;
		}
	}

	flush();
	*ptr = 0;

	return ptr - s;
}

std::string Dns::_read_dns_query_name(uint8* ptr, uint8* buf, uint8** next)
{
	std::string name;

	*next = ptr + 1;
	bool jumped = false;
	while (*ptr)
	{
		if (*ptr >= 192)
		{
			uint16& val = *reinterpret_cast<uint16*>(ptr);
			uint16 off = ntohs(val) & 0b111111'11111111;
			ptr = buf + off;
			jumped = true;
		}
		else
		{
			name += (char)*ptr++;
			*next += (int)!jumped;
		}
	}

	if (jumped)
	{
		++* next;
	}

	char* str = name.data();
	for (; *str;)
	{
		if (str != name.data())
		{
			*str++ = '.';
		}

		size_t cnt = *str;
		if (cnt == 0)
		{
			*--str = 0;
			name.resize(str - name.data());
			break;
		}

		memmove(str, str + 1, cnt);
		str += cnt;
	}

	return name;
}