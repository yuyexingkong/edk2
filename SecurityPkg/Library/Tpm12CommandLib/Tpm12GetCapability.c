/** @file
  Implement TPM1.2 Get Capabilities related commands.

Copyright (c) 2016, Intel Corporation. All rights reserved. <BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Library/Tpm12CommandLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/Tpm12DeviceLib.h>

#pragma pack(1)

typedef struct {
  TPM_RQU_COMMAND_HDR  Hdr;
  UINT32               Capability;
  UINT32               CapabilityFlagSize;
  UINT32               CapabilityFlag;
} TPM_CMD_GET_CAPABILITY;

typedef struct {
  TPM_RSP_COMMAND_HDR  Hdr;
  UINT32               ResponseSize;
  TPM_PERMANENT_FLAGS  Flags;
} TPM_RSP_GET_CAPABILITY_PERMANENT_FLAGS;

typedef struct {
  TPM_RSP_COMMAND_HDR  Hdr;
  UINT32               ResponseSize;
  TPM_STCLEAR_FLAGS    Flags;
} TPM_RSP_GET_CAPABILITY_STCLEAR_FLAGS;

#pragma pack()

/**
Get TPM capability permanent flags.

@param[out] TpmPermanentFlags   Pointer to the buffer for returned flag structure.

@retval EFI_SUCCESS           Operation completed successfully.
@retval EFI_TIMEOUT           The register can't run into the expected status in time.
@retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
@retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
Tpm12GetCapabilityFlagPermanent (
  OUT TPM_PERMANENT_FLAGS  *TpmPermanentFlags
  )
{
  EFI_STATUS                              Status;
  TPM_CMD_GET_CAPABILITY                  Command;
  TPM_RSP_GET_CAPABILITY_PERMANENT_FLAGS  Response;
  UINT32                                  Length;

  //
  // send Tpm command TPM_ORD_GetCapability
  //
  Command.Hdr.tag            = SwapBytes16 (TPM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize      = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal        = SwapBytes32 (TPM_ORD_GetCapability);
  Command.Capability         = SwapBytes32 (TPM_CAP_FLAG);
  Command.CapabilityFlagSize = SwapBytes32 (sizeof (TPM_CAP_FLAG_PERMANENT));
  Command.CapabilityFlag     = SwapBytes32 (TPM_CAP_FLAG_PERMANENT);
  Length = sizeof (Response);
  Status = Tpm12SubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ZeroMem (TpmPermanentFlags, sizeof (*TpmPermanentFlags));
  CopyMem (TpmPermanentFlags, &Response.Flags, MIN (sizeof (*TpmPermanentFlags), Response.ResponseSize));

  return Status;
}

/**
Get TPM capability volatile flags.

@param[out] VolatileFlags   Pointer to the buffer for returned flag structure.

@retval EFI_SUCCESS      Operation completed successfully.
@retval EFI_DEVICE_ERROR The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
Tpm12GetCapabilityFlagVolatile (
  OUT TPM_STCLEAR_FLAGS  *VolatileFlags
  )
{
  EFI_STATUS                            Status;
  TPM_CMD_GET_CAPABILITY                Command;
  TPM_RSP_GET_CAPABILITY_STCLEAR_FLAGS  Response;
  UINT32                                Length;

  //
  // send Tpm command TPM_ORD_GetCapability
  //
  Command.Hdr.tag            = SwapBytes16 (TPM_TAG_RQU_COMMAND);
  Command.Hdr.paramSize      = SwapBytes32 (sizeof (Command));
  Command.Hdr.ordinal        = SwapBytes32 (TPM_ORD_GetCapability);
  Command.Capability         = SwapBytes32 (TPM_CAP_FLAG);
  Command.CapabilityFlagSize = SwapBytes32 (sizeof (TPM_CAP_FLAG_VOLATILE));
  Command.CapabilityFlag     = SwapBytes32 (TPM_CAP_FLAG_VOLATILE);
  Length = sizeof (Response);
  Status = Tpm12SubmitCommand (sizeof (Command), (UINT8 *)&Command, &Length, (UINT8 *)&Response);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ZeroMem (VolatileFlags, sizeof (*VolatileFlags));
  CopyMem (VolatileFlags, &Response.Flags, MIN (sizeof (*VolatileFlags), Response.ResponseSize));

  return Status;
}