// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "AssemblyInfo.Sockets.h"

DEFINE_ASSEMBLY_INFO;

GENERATE_BODY(libty::Sockets::Net::EAddressFamily);
GENERATE_BODY(libty::Sockets::Net::EProtocolType);
GENERATE_BODY(libty::Sockets::Net::SNetUtility);

GENERATE_BODY(libty::Sockets::ESocketType);
GENERATE_BODY(libty::Sockets::ESocketFlags);
GENERATE_BODY(libty::Sockets::SSocket);

GENERATE_BODY(libty::Sockets::Web::EHttpStatusCode);