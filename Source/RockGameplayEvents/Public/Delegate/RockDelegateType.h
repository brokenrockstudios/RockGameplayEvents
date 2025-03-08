// Copyright Broken Rock Studios LLC. All Rights Reserved.
// See the LICENSE file for details.

#pragma once

UENUM()
enum class ERockDelegateType : uint8
{
	// This is a regular Multicast Delegate
	Regular,
	// This is a Sparse Delegate
	Sparse,

	// Unknown,
	// MulticastDelegate, // FMulticastDelegateProperty
	// MulticastInline,   // FMulticastInlineDelegateProperty
	// MulticastSparse,   // FMulticastSparseDelegateProperty
	// BlueprintDelegate, // MulticastScript?  Is this or regular just the regularDelegate?
	// Multicast can't have return values, but other delegates can have return values
	RockDelegateType_MAX UMETA(Hidden)
};

