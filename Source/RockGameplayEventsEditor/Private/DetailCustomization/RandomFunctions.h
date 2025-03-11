// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.


// Need better home/place

#pragma once

class RockCustomizationFunctions
{
public:
	static FText GetArrayTextValue(uint32 NumElements)
	{
		return NumElements > 1
			       ? FText::Format(NSLOCTEXT("RockGameplayEvents", "NumArrayItemsFmt", "{0} Array elements"), FText::AsNumber(NumElements))
			       : FText::Format(NSLOCTEXT("RockGameplayEvents", "SingleArrayItemFmt", "{0} Array element"), FText::AsNumber(NumElements));
	}
};
