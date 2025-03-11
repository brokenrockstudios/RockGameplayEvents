#include "Delegate/DelegateFunctionInfo.h"

FString FRockDelegateInfo::GetName() const
{
	return Name;
}

FString FRockDelegateInfo::GetNameWithClass() const
{
	return (DefiningClass ? DefiningClass->GetName() : "") + "::" + Name;
}

UFunction* FRockDelegateInfo::GetSignatureFunction() const
{
	return SignatureFunction.Get();
}


UFunction* FRockFunctionInfo::GetSignatureFunction() const
{
	return Function.Get();
}


// FString FRockFunctionInfo::GetSignatureFunctionString() const
// {
// 	// get all the parameter types and put them in a foo(int, float, bool) format
//
// 	auto parameters = UMiscHelperFunctions::BuildFunctionParameterString(Function, false);
// 	if (parameters.Len() > 0)
// 	{
// 		parameters = "(" + parameters + ")";
// 	}
// 	const FString SignatureString = Name + parameters;
// 	return SignatureString;
// }



// FString FRockDelegateInfo::GetSignatureParametersString() const
// {
// 	auto parameters = UMiscHelperFunctions::BuildFunctionParameterString(SignatureFunction, true);
// 	if (parameters.Len() > 0)
// 	{
// 		parameters = "(" + parameters + ")";
// 	}
// 	return parameters;
// }

// FString FRockDelegateInfo::GetSignatureFunctionString() const
// {
// 	const FString SignatureString = Name + GetSignatureParametersString();
// 	return SignatureString;
// }

// FString FRockDelegateInfo::GetDelegateTypeString() const
// {
// 	switch (DelegateType)
// 	{
// 	case ERockDelegateType::MulticastDelegate:
// 		return "Multicast Delegate";
// 	case ERockDelegateType::BlueprintDelegate:
// 		return "Blueprint Delegate";
// 	default:
// 		return "Unknown";
// 	}
// }