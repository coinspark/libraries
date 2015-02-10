/*
 * CoinSpark 2.1 - C library
 *
 * Copyright (c) Coin Sciences Ltd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef _COINSPARK_H_
#define _COINSPARK_H_

#include <stdlib.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif
    

// CoinSpark constants and other macros
    
#define TRUE 1
#define FALSE 0
    
#define COINSPARK_MIN(a,b) (((a)<(b)) ? (a) : (b))
#define COINSPARK_MAX(a,b) (((a)>(b)) ? (a) : (b))
    
#define COINSPARK_SATOSHI_QTY_MAX 2100000000000000
#define COINSPARK_ASSET_QTY_MAX 100000000000000
#define COINSPARK_PAYMENT_REF_MAX 0xFFFFFFFFFFFFF // 2^52-1
  
#define COINSPARK_GENESIS_QTY_MANTISSA_MIN 1
#define COINSPARK_GENESIS_QTY_MANTISSA_MAX 1000
#define COINSPARK_GENESIS_QTY_EXPONENT_MIN 0
#define COINSPARK_GENESIS_QTY_EXPONENT_MAX 11
#define COINSPARK_GENESIS_CHARGE_FLAT_MAX 5000
#define COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN 0
#define COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX 100
#define COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX 50
#define COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN 0
#define COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX 2
#define COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN 0
#define COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX 250
#define COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN 32
#define COINSPARK_GENESIS_PAGE_PATH_MAX_LEN 24
#define COINSPARK_GENESIS_HASH_MIN_LEN 12
#define COINSPARK_GENESIS_HASH_MAX_LEN 32

#define COINSPARK_ASSETREF_BLOCK_NUM_MAX 4294967295
#define COINSPARK_ASSETREF_TX_OFFSET_MAX 4294967295
#define COINSPARK_ASSETREF_TXID_PREFIX_LEN 2

#define COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE -1 // magic number for a default route
    
#define COINSPARK_MESSAGE_SERVER_HOST_MAX_LEN 32
#define COINSPARK_MESSAGE_SERVER_PATH_MAX_LEN 24
#define COINSPARK_MESSAGE_HASH_MIN_LEN 12
#define COINSPARK_MESSAGE_HASH_MAX_LEN 32
#define COINSPARK_MESSAGE_MAX_IO_RANGES 16

#define COINSPARK_IO_INDEX_MAX 65535

#define COINSPARK_ADDRESS_FLAG_ASSETS 1
#define COINSPARK_ADDRESS_FLAG_PAYMENT_REFS 2
#define COINSPARK_ADDRESS_FLAG_TEXT_MESSAGES 4
#define COINSPARK_ADDRESS_FLAG_FILE_MESSAGES 8
#define COINSPARK_ADDRESS_FLAG_MASK 0x7FFFFF // 23 bits are currently usable

// CoinSpark type definitions

#ifndef __cplusplus
    typedef char bool;
#endif
    
typedef int64_t CoinSparkSatoshiQty;
typedef int64_t CoinSparkAssetQty;
typedef int32_t CoinSparkIOIndex;
typedef int32_t CoinSparkAddressFlags;
typedef int64_t CoinSparkPaymentRef;
    
typedef struct {
    char bitcoinAddress[64];
    CoinSparkAddressFlags addressFlags;
    CoinSparkPaymentRef paymentRef;
} CoinSparkAddress;
    
typedef struct {
    int16_t qtyMantissa;
    int16_t qtyExponent;
    int16_t chargeFlatMantissa;
    int16_t chargeFlatExponent;
    int16_t chargeBasisPoints; // one hundredths of a percent
    bool useHttps;
    char domainName[COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN+1]; // null terminated
    bool usePrefix; // prefix coinspark/ in asset web page URL path
    char pagePath[COINSPARK_GENESIS_PAGE_PATH_MAX_LEN+1]; // null terminated
    int8_t assetHash[COINSPARK_GENESIS_HASH_MAX_LEN];
    size_t assetHashLen; // number of bytes in assetHash that are valid for comparison/encoding
} CoinSparkGenesis;

typedef struct {
    int64_t blockNum; // block in which genesis transaction is confirmed
    int64_t txOffset; // byte offset within that block
    u_int8_t txIDPrefix[COINSPARK_ASSETREF_TXID_PREFIX_LEN]; // first bytes of genesis transaction id
} CoinSparkAssetRef;

typedef struct {
    CoinSparkIOIndex first, count;
} CoinSparkIORange;

typedef struct {
    CoinSparkAssetRef assetRef;
    CoinSparkIORange inputs;
    CoinSparkIORange outputs;
    CoinSparkAssetQty qtyPerOutput;
} CoinSparkTransfer;
    
typedef struct {
    bool useHttps;
    char serverHost[COINSPARK_MESSAGE_SERVER_HOST_MAX_LEN+1]; // null terminated
    bool usePrefix; // prefix coinspark/ in server path
    char serverPath[COINSPARK_MESSAGE_SERVER_PATH_MAX_LEN+1]; // null terminated
    bool isPublic; // is the message publicly viewable
    int countOutputRanges; // number of elements in output range array
    CoinSparkIORange outputRanges[COINSPARK_MESSAGE_MAX_IO_RANGES]; // array of output ranges
    int8_t hash[COINSPARK_MESSAGE_HASH_MAX_LEN];
    size_t hashLen; // number of bytes in hash that are valid for comparison/encoding
} CoinSparkMessage;
    
    
// General functions for managing CoinSpark metadata and bitcoin transaction output scripts
    
size_t CoinSparkScriptToMetadata(const char* scriptPubKey, const size_t scriptPubKeyLen, const bool scriptIsHex,
                                 char* metadata, const size_t metadataMaxLen);
    // Checks whether the bitcoin tx output script in scriptPubKey (byte length scriptPubKeyLen) contains OP_RETURN metadata.
    // If scriptIsHex is true, scriptPubKey is interpreted as hex text, otherwise as raw binary data.
    // If so, copy it to metadata (size metadataMaxLen) and return the number of bytes used.
    // If no OP_RETURN data is found or metadataMaxLen is too small, returns 0.
    
size_t CoinSparkScriptsToMetadata(const char* scriptPubKeys[], const size_t scriptPubKeyLens[], const bool scriptsAreHex,
                                  const int countScripts, char* metadata, const size_t metadataMaxLen);
    // Extracts the first OP_RETURN metadata from the array of bitcoin transaction scripts in scriptPubKeys
    // (array length countScripts) where each script's byte length is in the respective element of scriptPubKeyLens.
    // If scriptsAreHex is true, scriptPubKeys are interpreted as hex text, otherwise as raw binary data.
    // Copies the metadata from the first OP_RETURN to metadata (size metadataMaxLen) and return the number of bytes used.
    // If no OP_RETURN data is found or metadataMaxLen is too small, returns 0.
    
size_t CoinSparkMetadataToScript(const char* metadata, const size_t metadataLen,
                                 char* scriptPubKey, const size_t scriptPubKeyMaxLen, const bool toHexScript);
    // Converts the data in metadata (length metadataLen) to an OP_RETURN bitcoin tx output in scriptPubKey
    // (maximum byte size scriptPubKeyMaxLen). If toHexScript is true, outputs hex text, otherwise binary data.
    // Returns the number of bytes used in scriptPubKey, or 0 if scriptPubKeyMaxLen was too small.
    
size_t CoinSparkMetadataMaxAppendLen(char* metadata, const size_t metadataLen, const size_t metadataMaxLen);
    // Calculates the maximum length of CoinSpark metadata that can be appended to some existing CoinSpark metadata
    // to fit into a total of metadataMaxLen bytes. The existing metadata (size metadataLen) can itself already be
    // a combination of more than one CoinSpark metadata element. The calculation is not simply metadataMaxLen-metadataLen
    // because some space is saved when combining pieces of CoinSpark metadata together.

size_t CoinSparkMetadataAppend(char* metadata, const size_t metadataLen, const size_t metadataMaxLen,
                               const char* appendMetadata, const size_t appendMetadataLen);
    // Appends the CoinSpark metadata in appendMetadata (size appendMetadataLen) to the existing CoinSpark metadata,
    // whose current size is metadataLen and whose maximum size is metadataMaxLen.
    // Returns the new total size of the metadata, or 0 if there was insufficient space or another error.

bool CoinSparkScriptIsRegular(const char* scriptPubKey, const size_t scriptPubKeyLen, const bool scriptIsHex);
    // Returns whether the bitcoin tx output script scriptPubKey (byte length scriptPubKeyLen) is 'regular', i.e. not an OP_RETURN.
    // This function will declare empty scripts or invalid hex scripts as 'regular' as well, since they are not OP_RETURNs.
    // If scriptIsHex is true, scriptPubKey is interpreted as hex text, otherwise as raw binary data.
    // Use this to build outputsRegular arrays which are used by various functions below.


// Functions for managing CoinSpark addresses
    
void CoinSparkAddressClear(CoinSparkAddress* address);
    // Set all fields in address to their default/zero values, which are not necessarily valid.
    
bool CoinSparkAddressToString(const CoinSparkAddress* address, char* string, const size_t stringMaxLen);
    // Outputs the address to a null-delimited string (size stringMaxLen) for debugging.
    // Returns true if there was enough space (1024 bytes recommended) or false otherwise.

bool CoinSparkAddressIsValid(const CoinSparkAddress* address);
    // Returns true if all values in the address are in their permitted ranges, false otherwise.
    
bool CoinSparkAddressMatch(const CoinSparkAddress* address1, const CoinSparkAddress* address2);
    // Returns true if the two CoinSparkAddress structures are identical.
    
size_t CoinSparkAddressEncode(const CoinSparkAddress* address, char* string, const size_t stringMaxLen);
    // Encodes the fields in address to a null-delimited CoinSpark address string (length stringMaxLen).
    // Returns the size of the formatted string if successful, otherwise 0.
    
bool CoinSparkAddressDecode(CoinSparkAddress* address, const char* string, const size_t stringLen);
    // Decodes the CoinSpark address string (length stringLen) into the fields in address.
    // Returns true if the address could be successfully read, otherwise false.
    
    
// Functions for managing asset genesis metadata

void CoinSparkGenesisClear(CoinSparkGenesis *genesis);
    // Set all fields in genesis to their default/zero values, which are not necessarily valid.
    
bool CoinSparkGenesisToString(const CoinSparkGenesis *genesis, char* string, const size_t stringMaxLen);
    // Outputs the genesis to a null-delimited string (size stringMaxLen) for debugging.
    // Returns true if there was enough space (1024 bytes recommended) or false otherwise.
    
bool CoinSparkGenesisIsValid(const CoinSparkGenesis *genesis);
    // Returns true if all values in the genesis are in their permitted ranges, false otherwise.
    
bool CoinSparkGenesisMatch(const CoinSparkGenesis* genesis1, const CoinSparkGenesis* genesis2, const bool strict);
    // Returns true if the two CoinSparkGenesis structures are the same. If strict is true then
    // the qtyMantissa, qtyExponent, chargeFlatMantissa and chargeFlatExponent fields must be identical.
    // If strict is false then it is enough if each pair just represents the same final quantity.

CoinSparkAssetQty CoinSparkGenesisGetQty(const CoinSparkGenesis *genesis);
    // Returns the number of units denoted by the genesis qtyMantissa and qtyExponent fields.
    
CoinSparkAssetQty CoinSparkGenesisSetQty(CoinSparkGenesis *genesis, const CoinSparkAssetQty desiredQty, const int rounding);
    // Sets the qtyMantissa and qtyExponent fields in genesis to be as close to desiredQty as possible.
    // Set rounding to [-1, 0, 1] for rounding [down, closest, up] respectively.
    // Returns the quantity that was actually encoded, via CoinSparkGenesisGetQty().
    
CoinSparkAssetQty CoinSparkGenesisGetChargeFlat(const CoinSparkGenesis *genesis);
    // Returns the number of units denoted by the genesis chargeFlatMantissa and chargeFlatExponent fields.
    
CoinSparkAssetQty CoinSparkGenesisSetChargeFlat(CoinSparkGenesis *genesis, CoinSparkAssetQty desiredChargeFlat, const int rounding);
    // Sets the chargeFlatMantissa and chargeFlatExponent fields in genesis to be as close to desiredChargeFlat as possible.
    // Set rounding to [-1, 0, 1] for rounding [down, closest, up] respectively.
    // Returns the quantity that was actually encoded, via CoinSparkGenesisGetChargeFlat().

CoinSparkAssetQty CoinSparkGenesisCalcCharge(const CoinSparkGenesis *genesis, CoinSparkAssetQty qtyGross);
    // Calculates the payment charge specified by genesis for sending the raw quantity qtyGross.
    
CoinSparkAssetQty CoinSparkGenesisCalcNet(const CoinSparkGenesis *genesis, CoinSparkAssetQty qtyGross);
    // Calculates the quantity that will be received after the payment charge specified by genesis is applied to qtyGross.

CoinSparkAssetQty CoinSparkGenesisCalcGross(const CoinSparkGenesis *genesis, CoinSparkAssetQty qtyNet);
    // Calculates the quantity that should be sent so that, after the payment charge specified by genesis
    // is applied, the recipient will receive qtyNet units.
    
size_t CoinSparkGenesisCalcHashLen(const CoinSparkGenesis *genesis, const size_t metadataMaxLen);
    // Calculates the appropriate asset hash length of genesis so that when encoded as metadata the genesis will
    // fit in metadataMaxLen bytes. For now, set metadataMaxLen to 40 (see Bitcoin's MAX_OP_RETURN_RELAY parameter).


// Functions for encoding, decoding and calculating minimum fees for CoinSpark genesis transactions
    
size_t CoinSparkGenesisEncode(const CoinSparkGenesis *genesis, char* metadata, const size_t metadataMaxLen);
    // Encodes genesis into metadata (size metadataMaxLen).
    // If the encoding was successful, returns the number of bytes used, otherwise 0.

bool CoinSparkGenesisDecode(CoinSparkGenesis* genesis, const char* metadata, const size_t metadataLen);
    // Decodes the data in metadata (length metadataLen) into genesis.
    // Return true if the decode was successful, false otherwise.
    
CoinSparkSatoshiQty CoinSparkGenesisCalcMinFee(const CoinSparkGenesis *genesis, const CoinSparkSatoshiQty* outputsSatoshis,
                                               const bool* outputsRegular, const int countOutputs);
    // Returns the minimum transaction fee (in bitcoin satoshis) required to make the genesis transaction valid.
    // Pass the number of bitcoin satoshis in each output in outputsSatoshis (array size countOutputs).
    // Use CoinSparkScriptIsRegular() to pass an array of bools in outputsRegular for whether each output script is regular.
    
    
// Functions for managing asset references
    
void CoinSparkAssetRefClear(CoinSparkAssetRef *assetRef);
    // Set all fields in assetRef to their default/zero values, which are not necessarily valid.
    
bool CoinSparkAssetRefToString(const CoinSparkAssetRef *assetRef, char* string, const size_t stringMaxLen);
    // Outputs the assetRef to a null-delimited string (size stringMaxLen) for debugging.
    // Returns true if there was enough space (1024 bytes recommended) or false otherwise.

bool CoinSparkAssetRefIsValid(const CoinSparkAssetRef *assetRef);
    // Returns true if all values in the asset reference are in their permitted ranges, false otherwise.    
    
bool CoinSparkAssetRefMatch(const CoinSparkAssetRef* assetRef1, const CoinSparkAssetRef* assetRef2);
    // Returns true if the two CoinSparkAssetRef structures are identical.

size_t CoinSparkAssetRefEncode(const CoinSparkAssetRef* assetRef, char* string, const size_t stringMaxLen);
    // Encodes the assetRef to a null-delimited CoinSpark asset reference string (length stringMaxLen).
    // Returns the size of the formatted string if successful, otherwise 0.
    
bool CoinSparkAssetRefDecode(CoinSparkAssetRef *assetRef, const char* string, const size_t stringLen);
    // Decodes the CoinSpark asset reference string (length stringLen) into assetRef.
    // Returns true if the asset reference could be successfully read, otherwise false.
    

    
// Functions for managing asset transfer metadata and arrays thereof
    
void CoinSparkTransferClear(CoinSparkTransfer* transfer);
    // Set all fields in transfer to their default/zero values, which are not necessarily valid.
    
bool CoinSparkTransferToString(const CoinSparkTransfer* transfer, char* string, const size_t stringMaxLen);
    // Outputs the transfer to a null-delimited string (size stringMaxLen) for debugging.
    // Returns true if there was enough space (1024 bytes recommended) or false otherwise.
    
bool CoinSparkTransfersToString(const CoinSparkTransfer* transfers, const int countTransfers, char* string, const size_t stringMaxLen);
    // Outputs the array of transfers to a null-delimited string (size stringMaxLen) for debugging.
    // Returns true if there was enough space (1024 * countTransfers bytes recommended) or false otherwise.
    
bool CoinSparkTransferMatch(const CoinSparkTransfer* transfer1, const CoinSparkTransfer* transfer2);
    // Returns true if the two CoinSparkTransfer structures are identical.
    
bool CoinSparkTransfersMatch(const CoinSparkTransfer* transfers1, const CoinSparkTransfer* transfers2, int countTransfers, bool strict);
    // Returns true if the two arrays of transfers in transfers1 and transfers2 are the same.
    // If strict is true then the ordering in the two arrays must be identical. If strict is false
    // then it is enough if each list is equivalent, i.e. the same transfers in the same order for each asset reference.

bool CoinSparkTransferIsValid(const CoinSparkTransfer* transfer);
    // Returns true is all values in the transfer are in their permitted ranges, false otherwise.
    
bool CoinSparkTransfersAreValid(const CoinSparkTransfer* transfers, const int countTransfers);
    // Returns true if all values in the array of transfers (size countTransfers) are valid.
    

// Functions for encoding, decoding and calculating minimum fees for CoinSpark transfer transactions

size_t CoinSparkTransfersEncode(const CoinSparkTransfer* transfers, const int countTransfers,
                                const int countInputs, const int countOutputs,
                                char* metadata, const size_t metadataMaxLen);
    // Encodes the array of transfers (length countTransfers) into metadata (whose size is metadataMaxLen).
    // Pass the number of transaction inputs and outputs in countInputs and countOutputs respectively.
    // If the encoding was successful, returns the number of bytes used, otherwise 0.
    
int CoinSparkTransfersDecodeCount(const char* metadata, const size_t metadataLen);
    // Returns the number of transfers encoded in metadata (length metadataLen).

int CoinSparkTransfersDecode(CoinSparkTransfer* transfers, const int maxTransfers,
                             const int countInputs, const int countOutputs,
                             const char* metadata, const size_t metadataLen);
    // Decodes up to maxTransfers transfers from metadata (length metadataLen) into the transfers array.
    // Pass the number of transaction inputs and outputs in countInputs and countOutputs respectively.
    // Returns total number of transfers encoded in metadata like CoinSparkTransfersDecodeCount() or 0 if an error.
    // If this return value is more than maxTransfers, try calling this again with a larger array.

CoinSparkSatoshiQty CoinSparkTransfersCalcMinFee(const CoinSparkTransfer* transfers, const int countTransfers,
                                                 const int countInputs, const int countOutputs,
                                                 const CoinSparkSatoshiQty* outputsSatoshis, const bool* outputsRegular);
    // Returns the minimum transaction fee (in bitcoin satoshis) required to make the set of transfers (array size countTransfers) valid.
    // Pass the number of transaction inputs and outputs in countInputs and countOutputs respectively.
    // Pass the number of bitcoin satoshis in each output in outputsSatoshis (array size countOutputs).
    // Use CoinSparkScriptIsRegular() to pass an array of bools in outputsRegular for whether each output script is regular.

    
// Functions for calculating asset quantities and change outputs
    
void CoinSparkGenesisApply(const CoinSparkGenesis* genesis, const bool* outputsRegular,
                           CoinSparkAssetQty* outputBalances, const int countOutputs);
    // For the asset specified by genesis, calculate the number of newly created asset units in each
    // output of the genesis transaction into the outputBalances array (size countOutputs).
    // Use CoinSparkScriptIsRegular() to pass an array of bools in outputsRegular for whether each output script is regular.
    // ** This is only relevant if the transaction DOES HAVE a sufficient fee to make the genesis valid **
    
void CoinSparkTransfersApply(const CoinSparkAssetRef* assetRef, const CoinSparkGenesis* genesis,
                             const CoinSparkTransfer* transfers, const int countTransfers,
                             const CoinSparkAssetQty* inputBalances, const int countInputs,
                             const bool* outputsRegular, CoinSparkAssetQty* outputBalances, const int countOutputs);
    // For the asset specified by assetRef and genesis, and list of transfers (size countTransfers), applies those transfers
    // to move units of that asset from inputBalances (size countInputs) to outputBalances (size countOutputs).
    // Only transfers whose assetRef matches the function's assetRef parameter will be applied (apart from default routes).
    // Use CoinSparkScriptIsRegular() to pass an array of bools in outputsRegular for whether each output script is regular.
    // ** Call this if the transaction DOES HAVE a sufficient fee to make the list of transfers valid **
    
void CoinSparkTransfersApplyNone(const CoinSparkAssetRef* assetRef, const CoinSparkGenesis* genesis,
                                 const CoinSparkAssetQty* inputBalances, const int countInputs,
                                 const bool* outputsRegular, CoinSparkAssetQty* outputBalances, const int countOutputs);
    // For the asset specified by assetRef and genesis, move units of that asset from inputBalances (size countInputs) to
    // outputBalances (size countOutputs), applying the default behavior only (all assets goes to last regular output).
    // This is equivalent to calling CoinSparkTransfersApply() with countTransfers=0.
    // ** Call this if the transaction DOES NOT HAVE a sufficient fee to make the list of transfers valid **

void CoinSparkTransfersDefaultOutputs(const CoinSparkTransfer* transfers, const int countTransfers, const int countInputs,
                                      const bool* outputsRegular, bool* outputsDefault, const int countOutputs);
    // For the list of transfers (size countTransfers) on a transaction with countInputs inputs, calculate
    // the array of bools in outputsDefault (size countOutputs) where each entry indicates whether that
    // output might receive some assets due to default routes. 
    
    
// Functions for managing payment references
    
bool CoinSparkPaymentRefToString(const CoinSparkPaymentRef paymentRef, char* string, const size_t stringMaxLen);
    // Outputs the paymentRef to a null-delimited string (size stringMaxLen) for debugging.
    // Returns true if there was enough space (1024 bytes recommended) or false otherwise.

bool CoinSparkPaymentRefIsValid(const CoinSparkPaymentRef paymentRef);
    // Returns true if paymentRef is in the permitted range, false otherwise.

CoinSparkPaymentRef CoinSparkPaymentRefRandom();
    // Returns a random payment reference that can be used for a CoinSpark address and embedded in a transaction
    
size_t CoinSparkPaymentRefEncode(const CoinSparkPaymentRef paymentRef, char* metadata, const size_t metadataMaxLen);
    // Encodes the paymentRef into metadata (whose size is metadataMaxLen);
    // If the encoding was successful, returns the number of bytes used, otherwise 0.
    
bool CoinSparkPaymentRefDecode(CoinSparkPaymentRef* paymentRef, const char* metadata, const size_t metadataLen);
    // Decodes the payment reference in metadata (length metadataLen) into paymentRef.
    // Return true if the decode was successful, false otherwise.
    
    
// Functions for managing messages

void CoinSparkMessageClear(CoinSparkMessage* message);
    // Set all fields in message to their default/zero values, which are not necessarily valid.
    
bool CoinSparkMessageToString(const CoinSparkMessage* message, char* string, const size_t stringMaxLen);
    // Outputs the message to a null-delimited string (size stringMaxLen) for debugging.
    // Returns true if there was enough space (4096 bytes recommended) or false otherwise.

bool CoinSparkMessageIsValid(const CoinSparkMessage* message);
    // Returns true if message is valid, false otherwise.

bool CoinSparkMessageMatch(const CoinSparkMessage* message1, const CoinSparkMessage* message2, const bool strict);
    // Returns true if the two CoinSparkMessage structures are the same. If strict is true then the outputRange
    // fields must be identical. If strict is false then they must only represent the same set of outputs.

size_t CoinSparkMessageEncode(const CoinSparkMessage* message, const int countOutputs, char* metadata, const size_t metadataMaxLen);
    // Encodes the message into metadata (whose size is metadataMaxLen)
    // Pass the number of transaction outputs in countOutputs respectively.
    // If the encoding was successful, returns the number of bytes used, otherwise 0.

bool CoinSparkMessageDecode(CoinSparkMessage* message, const int countOutputs, const char* metadata, const size_t metadataLen);
    // Decodes the message in metadata (length metadataLen) into the message variable.
    // Pass the number of transaction outputs in countOutputs respectively.
    // Returns true if the decode was successful, false otherwise.
    
bool CoinSparkMessageHasOutput(const CoinSparkMessage* message, int outputIndex);
    // Returns true if the message is intended for the given outputIndex.

size_t CoinSparkMessageCalcHashLen(const CoinSparkMessage *message, int countOutputs, const size_t metadataMaxLen);
    // Calculates the appropriate hash length for message so that when encoded as metadata the message will
    // fit in metadataMaxLen bytes. For now, set metadataMaxLen to 40 (see Bitcoin's MAX_OP_RETURN_RELAY parameter).

    
// Functions for calculating URLs and hashes
    
size_t CoinSparkGenesisCalcAssetURL(const CoinSparkGenesis *genesis, const char* firstSpentTxID, const int firstSpentVout,
                                    char* urlString, const size_t urlStringMaxLen);
    // Calculates the URL for the asset web page of genesis into urlString (length urlStringMaxLen).
    // In firstSpentTxID, pass the previous txid whose output was spent by the first input of the genesis.
    // In firstSpentVout, pass the output index of firstSpentTxID spent by the first input of the genesis.
    // Returns the length of the URL, or 0 if urlStringMaxLen (recommended 256 bytes) was not large enough.
    
size_t CoinSparkMessageCalcServerURL(const CoinSparkMessage* message, char* urlString, const size_t urlStringMaxLen);
    // Calculates the URL for the message server into urlString (length urlStringMaxLen).
    // Returns the length of the URL, or 0 if urlStringMaxLen (recommended 256 bytes) was not large enough.
    
void CoinSparkCalcAssetHash(const char* name, size_t nameLen,
                            const char* issuer, size_t issuerLen,
                            const char* description, size_t descriptionLen,
                            const char* units, size_t unitsLen,
                            const char* issueDate, size_t issueDateLen,
                            const char* expiryDate, size_t expiryDateLen,
                            const double* interestRate, const double* multiple,
                            const char* contractContent, const size_t contractContentLen,
                            unsigned char assetHash[32]);
    // Calculates the assetHash for the key information from a CoinSpark asset web page JSON specification.
    // All char* string parameters except contractContent must be passed using UTF-8 encoding.
    // You may pass NULL (and if appropriate, a length of zero) for any parameter which was not in the JSON.
    // Note that you need to pass in the contract *content* and length, not its URL.
    
typedef struct {
    char* mimeType;
    size_t mimeTypeLen;
    char* fileName; // can be NULL
    size_t fileNameLen;
    unsigned char* content;
    size_t contentLen;
} CoinSparkMessagePart;

void CoinSparkCalcMessageHash(const unsigned char* salt, size_t saltLen, const CoinSparkMessagePart* messageParts,
                              const int countParts, unsigned char messageHash[32]);
    // Calculates the messageHash for a CoinSpark message containing the given messageParts array (length countParts).
    // Pass in a random string in salt (length saltLen), that should be sent to the message server along with the content.

void CoinSparkCalcSHA256Hash(const unsigned char* input, const size_t inputLen, unsigned char hash[32]);
    // Calculates the SHA-256 hash of the raw data in input (size inputLen) and places it in the hash variable.

    
// Functions whose purpose is to lock down the legal definitions in CoinSpark contracts

char* CoinSparkGetGenesisWebPageURL(const char* scriptPubKeys[], const size_t scriptPubKeysLen[], const int countOutputs,
                                    u_int8_t firstSpentTxId[32], const int firstSpentVout);
    
CoinSparkAssetQty CoinSparkGetGenesisOutputQty(const char* scriptPubKeys[], const size_t scriptPubKeysLen[],
                                               const CoinSparkSatoshiQty* outputsSatoshis, const int countOutputs,
                                               const CoinSparkSatoshiQty transactionFee, const int getOutputIndex);

CoinSparkAssetQty CoinSparkGetTransferOutputQty(const char* genesisScriptPubKeys[], const size_t genesisScriptPubKeysLen[],
                                                const CoinSparkSatoshiQty* genesisOutputsSatoshis, const int genesisCountOutputs,
                                                const CoinSparkSatoshiQty genesisTransactionFee,
                                                int64_t genesisBlockNum, int64_t genesisTxOffset, u_int8_t genesisTxId[32],
                                                const CoinSparkAssetQty* thisInputBalances, const int thisCountInputs,
                                                const char* thisScriptPubKeys[], const size_t thisScriptPubKeysLen[],
                                                const CoinSparkSatoshiQty* thisOutputsSatoshis, const int thisCountOutputs,
                                                const CoinSparkSatoshiQty thisTransactionFee, const int getOutputIndex);
    

#ifdef __cplusplus
}
#endif

#endif // _COINSPARK_H_
