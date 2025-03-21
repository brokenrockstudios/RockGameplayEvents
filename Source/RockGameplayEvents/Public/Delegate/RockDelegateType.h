// Copyright 2025 Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "RockDelegateType.generated.h"

UENUM()
enum class ERockDelegateType : uint8
{
	// This is a regular Multicast Delegate
	MulticastDelegate,
	// Non multicast delegate
	BlueprintDelegate,

	
	// This is a Sparse Delegate.  It appears like we might not need to differentiate on this?
	// Sparse,

	// Unknown,
	// MulticastDelegate, // FMulticastDelegateProperty
	// MulticastInline,   // FMulticastInlineDelegateProperty
	// MulticastSparse,   // FMulticastSparseDelegateProperty
	// BlueprintDelegate, // MulticastScript?  Is this or regular just the regularDelegate?
	// Multicast can't have return values, but other delegates can have return values
	RockDelegateType_MAX UMETA(Hidden)
};

