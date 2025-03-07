#include "Delegate/DelegateFunctionInfo.h"

#include "Misc/MiscHelperFunctions.h"

FText FRockDelegateInfo::GetNameWithClass() const
{
	return FText::FromString((DefiningClass ? DefiningClass->GetName() : "") + "::" + Name);
}

FText FRockDelegateInfo::GetDisplayName() const
{
	// check UDeveloperSettings if we show GetName or GetSignatureFunctionString

	if (false
		//&& GetDefault<URockGameplayEventDeveloperSettings>()->ShowFullFunctionSignature
	)
	{
		return GetSignatureFunctionString();
	}
	else
	{
		return GetName();
	}
}

FText FRockDelegateInfo::GetSignatureFunctionString() const
{
	// get all the parameter types and put them in a foo(int, float, bool) format
	const FString SignatureString = Name + "(" + UMiscHelperFunctions::BuildFunctionParameterString(SignatureFunction, true, false, false) + ")";
	return FText::FromString(SignatureString);
}

FText FRockDelegateInfo::GetName() const
{
	return FText::FromString(Name);
}
