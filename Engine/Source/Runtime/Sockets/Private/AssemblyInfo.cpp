// Copyright 2020-2022 Aumoa.lib. All right reserved.

DEFINE_ASSEMBLY_INFO;

#include "Net/AddressFamily.h"
#include "Net/ProtocolType.h"
#include "Net/NetUtility.h"

GENERATE_BODY(libty::Sockets::Net::EAddressFamily);
GENERATE_BODY(libty::Sockets::Net::EProtocolType);
GENERATE_BODY(libty::Sockets::Net::SNetUtility);

#include "Sockets/SocketType.h"
#include "Sockets/SocketFlags.h"
#include "Sockets/Socket.h"

GENERATE_BODY(libty::Sockets::ESocketType);
GENERATE_BODY(libty::Sockets::ESocketFlags);
GENERATE_BODY(libty::Sockets::SSocket);

#include "Web/HttpStatusCode.h"

GENERATE_BODY(libty::Sockets::Web::EHttpStatusCode);