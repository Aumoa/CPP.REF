// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CompiledSMEMBER.h"

CompiledSMEMBER::CompiledSMEMBER()
{
}

void CompiledSMEMBER::Generate(const String& owningType, size_t indent, StringBuilder& sb)
{
	String safeMemberName = MemberName.Replace(TEXT("~"), TEXT("_"));

	String indentStr = String('\t', indent);
	sb.AppendLine(TEXTF("{}struct reflexpr_{}_{} \\", indentStr, owningType, safeMemberName));
	sb.AppendLine(TEXTF("{}{{ \\", indentStr));
	switch (eBodyAccessKey)
	{
	case EBodyAccessKey::Public:
		sb.AppendLine(TEXTF("{}\tusing is_public_t = int; \\", indentStr));
		break;
	case EBodyAccessKey::Protected:
		sb.AppendLine(TEXTF("{}\tusing is_protected_t = int; \\", indentStr));
		break;
	case EBodyAccessKey::Private:
		sb.AppendLine(TEXTF("{}\tusing is_private_t = int; \\", indentStr));
		break;
	}
	sb.AppendLine(TEXTF("{}}}; \\", indentStr));
	sb.AppendLine(TEXTF("{} \\", indentStr));
}