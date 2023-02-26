// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/StaticClass.h"
#include "Net/IPHostEntry.h"
#include "Threading/Tasks/Task.h"

class CORE_API Dns : public StaticClass
{
private:
	static constexpr int32 T_A = 1; //Ipv4 address
	static constexpr int32 T_NS = 2; //Nameserver
	static constexpr int32 T_CNAME = 5; // canonical name
	static constexpr int32 T_SOA = 6; /* start of authority zone */
	static constexpr int32 T_PTR = 12; /* domain name pointer */
	static constexpr int32 T_MX = 15; //Mail server

	//DNS header structure
	struct DNS_HEADER
	{
		uint16 id; // identification number

		uint8 rd : 1; // recursion desired
		uint8 tc : 1; // truncated message
		uint8 aa : 1; // authoritive answer
		uint8 opcode : 4; // purpose of message
		uint8 qr : 1; // query/response flag

		uint8 rcode : 4; // response code
		uint8 cd : 1; // checking disabled
		uint8 ad : 1; // authenticated data
		uint8 z : 1; // its z! reserved
		uint8 ra : 1; // recursion available

		uint16 q_count; // number of question entries
		uint16 ans_count; // number of answer entries
		uint16 auth_count; // number of authority entries
		uint16 add_count; // number of resource entries
	};

	//Constant sized fields of query structure
	struct QUESTION
	{
		uint16 qtype;
		uint16 qclass;
	};

	//Constant sized fields of the resource record structure
#pragma pack(push, 1)
	struct R_DATA
	{
		uint16 type;
		uint16 _class;
		unsigned int ttl;
		uint16 data_len;
	};
#pragma pack(pop)

	//Pointers to resource record contents
	struct RES_RECORD
	{
		std::string name;
		struct R_DATA* resource;
		std::string rdata;
	};

	//Structure of a Query
	struct QUERY
	{
		const char* name;
		struct QUESTION* ques;
	};

public:
	static Task<std::vector<IPHostEntry>> GetHostEntryAsync(String hostname, std::stop_token cancellationToken = {});

private:
	static size_t _calc_dns_query_name(uint8* ptr, std::string_view host);
	static std::string _read_dns_query_name(uint8* ptr, uint8* buf, uint8** next);
};