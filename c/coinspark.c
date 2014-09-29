/*
 * CoinSpark 1.0 - C library
 *
 * Copyright (c) 2014 Coin Sciences Ltd
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


#include "coinspark.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

// Macros used internally

#define COINSPARK_UNSIGNED_BYTE_MAX 0xFF
#define COINSPARK_UNSIGNED_2_BYTES_MAX 0xFFFF
#define COINSPARK_UNSIGNED_3_BYTES_MAX 0xFFFFFF
#define COINSPARK_UNSIGNED_4_BYTES_MAX 0xFFFFFFFF

#define COINSPARK_METADATA_IDENTIFIER "SPK"
#define COINSPARK_METADATA_IDENTIFIER_LEN 3
#define COINSPARK_LENGTH_PREFIX_MAX 96
#define COINSPARK_GENESIS_PREFIX 'g'
#define COINSPARK_TRANSFERS_PREFIX 't'
#define COINSPARK_PAYMENTREF_PREFIX 'r'

#define COINSPARK_FEE_BASIS_MAX_SATOSHIS 1000

#define COINSPARK_GENESIS_QTY_FLAGS_LENGTH 2
#define COINSPARK_GENESIS_QTY_MASK 0x3FFF
#define COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE 1001
#define COINSPARK_GENESIS_FLAG_CHARGE_FLAT 0x4000
#define COINSPARK_GENESIS_FLAG_CHARGE_BPS 0x8000
#define COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE 101
#define COINSPARK_GENESIS_CHARGE_FLAT_LENGTH 1
#define COINSPARK_GENESIS_CHARGE_BPS_LENGTH 1

#define COINSPARK_DOMAIN_PACKING_PREFIX_MASK 0xC0
#define COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT 6
#define COINSPARK_DOMAIN_PACKING_SUFFIX_MASK 0x3F
#define COINSPARK_DOMAIN_PACKING_SUFFIX_MAX 62
#define COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4 63
#define COINSPARK_DOMAIN_PACKING_IPv4_HTTPS 0x40

#define COINSPARK_DOMAIN_PATH_ENCODE_BASE 40
#define COINSPARK_DOMAIN_PATH_FALSE_END_CHAR '<'
#define COINSPARK_DOMAIN_PATH_TRUE_END_CHAR '>'

#define COINSPARK_PACKING_GENESIS_MASK 0xC0
#define COINSPARK_PACKING_GENESIS_PREV 0x00
#define COINSPARK_PACKING_GENESIS_3_3_BYTES 0x40 // 3 bytes for block index, 3 for txn offset
#define COINSPARK_PACKING_GENESIS_3_4_BYTES 0x80 // 3 bytes for block index, 4 for txn offset
#define COINSPARK_PACKING_GENESIS_4_4_BYTES 0xC0 // 4 bytes for block index, 4 for txn offset

#define COINSPARK_PACKING_INDICES_MASK 0x38
#define COINSPARK_PACKING_INDICES_0P_0P 0x00 // input 0 only or previous, output 0 only or previous
#define COINSPARK_PACKING_INDICES_0P_1S 0x08 // input 0 only or previous, output 1 only or subsequent single
#define COINSPARK_PACKING_INDICES_0P_ALL 0x10 // input 0 only or previous, all outputs
#define COINSPARK_PACKING_INDICES_1S_0P 0x18 // input 1 only or subsequent single, output 0 only or previous
#define COINSPARK_PACKING_INDICES_ALL_0P 0x20 // all inputs, output 0 only or previous
#define COINSPARK_PACKING_INDICES_ALL_1S 0x28 // all inputs, output 1 only or subsequent single
#define COINSPARK_PACKING_INDICES_ALL_ALL 0x30 // all inputs, all outputs
#define COINSPARK_PACKING_INDICES_EXTEND 0x38 // use second byte for more extensive information

#define COINSPARK_PACKING_EXTEND_INPUTS_SHIFT 3
#define COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT 0

#define COINSPARK_PACKING_EXTEND_MASK 0x07
#define COINSPARK_PACKING_EXTEND_0P 0x00 // index 0 only or previous
#define COINSPARK_PACKING_EXTEND_1S 0x01 // index 1 only or subsequent single
#define COINSPARK_PACKING_EXTEND_BYTE 0x02 // 1 byte for single index
#define COINSPARK_PACKING_EXTEND_2_BYTES 0x03 // 2 bytes for single index
#define COINSPARK_PACKING_EXTEND_1_1_BYTES 0x04 // 1 byte for first index, 1 byte for count
#define COINSPARK_PACKING_EXTEND_2_1_BYTES 0x05 // 2 bytes for first index, 1 byte for count
#define COINSPARK_PACKING_EXTEND_2_2_BYTES 0x06 // 2 bytes for first index, 2 bytes for count
#define COINSPARK_PACKING_EXTEND_ALL 0x07 // all inputs|outputs

#define COINSPARK_PACKING_QUANTITY_MASK 0x07
#define COINSPARK_PACKING_QUANTITY_1P 0x00 // quantity=1 or previous
#define COINSPARK_PACKING_QUANTITY_1_BYTE 0x01
#define COINSPARK_PACKING_QUANTITY_2_BYTES 0x02
#define COINSPARK_PACKING_QUANTITY_3_BYTES 0x03
#define COINSPARK_PACKING_QUANTITY_4_BYTES 0x04
#define COINSPARK_PACKING_QUANTITY_6_BYTES 0x05
#define COINSPARK_PACKING_QUANTITY_FLOAT 0x06
#define COINSPARK_PACKING_QUANTITY_MAX 0x07 // transfer all quantity across

#define COINSPARK_TRANSFER_QTY_FLOAT_LENGTH 2
#define COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX 1000
#define COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX 11
#define COINSPARK_TRANSFER_QTY_FLOAT_MASK 0x3FFF
#define COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE 1001

#define COINSPARK_ADDRESS_PREFIX 's'
#define COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE 10
#define COINSPARK_ADDRESS_CHAR_INCREMENT 13

// Type definitions and constants used internally

typedef enum { // options to use in order of priority
    firstPackingType=0,
    _0P=0,
    _1S,
    _ALL,
    _BYTE,
    _2_BYTES,
    _1_1_BYTES,
    _2_1_BYTES,
    _2_2_BYTES,
    countPackingTypes
} PackingType;

static const char packingExtendMap[]={ // same order as above
    COINSPARK_PACKING_EXTEND_0P,
    COINSPARK_PACKING_EXTEND_1S,
    COINSPARK_PACKING_EXTEND_ALL,
    COINSPARK_PACKING_EXTEND_BYTE,
    COINSPARK_PACKING_EXTEND_2_BYTES,
    COINSPARK_PACKING_EXTEND_1_1_BYTES,
    COINSPARK_PACKING_EXTEND_2_1_BYTES,
    COINSPARK_PACKING_EXTEND_2_2_BYTES
};

static const char domainPathChars[COINSPARK_DOMAIN_PATH_ENCODE_BASE+1]="0123456789abcdefghijklmnopqrstuvwxyz-.<>";
    // last two characters are end markers, < means false, > means true

static const char integerToBase58[58]="123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

static const int base58Minus49ToInteger[74]={
     0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1, -1,
     9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1, 22,
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1, -1,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57
};

static const char *domainNamePrefixes[]={
    "",
    "www."
};

static const char *domainNameSuffixes[60]={ // leave space for 3 more in future
    "",
    
    // most common suffixes based on Alexa's top 1M sites as of 10 June 2014, with some manual adjustments

    ".at",
    ".au",
    ".be",
    ".biz",
    ".br",
    ".ca",
    ".ch",
    ".cn",
    ".co.jp",
    ".co.kr",
    ".co.uk",
    ".co.za",
    ".co",
    ".com.ar",
    ".com.au",
    ".com.br",
    ".com.cn",
    ".com.mx",
    ".com.tr",
    ".com.tw",
    ".com.ua",
    ".com",
    ".cz",
    ".de",
    ".dk",
    ".edu",
    ".es",
    ".eu",
    ".fr",
    ".gov",
    ".gr",
    ".hk",
    ".hu",
    ".il",
    ".in",
    ".info",
    ".ir",
    ".it",
    ".jp",
    ".kr",
    ".me",
    ".mx",
    ".net",
    ".nl",
    ".no",
    ".org",
    ".pl",
    ".ps",
    ".ro",
    ".ru",
    ".se",
    ".sg",
    ".tr",
    ".tv",
    ".tw",
    ".ua",
    ".uk",
    ".us",
    ".vn"
};

typedef bool PackingOptions[countPackingTypes];

typedef struct {
    size_t  blockNumBytes, txOffsetBytes, txIDPrefixBytes, firstInputBytes,
            countInputsBytes, firstOutputBytes, countOutputsBytes, quantityBytes;
} PackingByteCounts;

// Functions used internally

static bool WriteSmallEndianUnsigned(const long long value, char* dataBuffer, size_t bytes)
{
    size_t byte;
    long long valueLeft;
    
    if (value<0)
        return FALSE; // does not support negative values
    
    valueLeft=value;
    
    for (byte=bytes; byte>0; byte--) {
        *dataBuffer++=(char)(valueLeft&0xFF);
        valueLeft/=256;
    }
    
    return !valueLeft; // if still something left, we didn't have enough bytes for representation
}

static long long ReadSmallEndianUnsigned(const char* dataBuffer, size_t bytes)
{
    long long value;
    const char *dataBufferPtr;
    
    value=0;
    
    for (dataBufferPtr=dataBuffer+bytes-1; dataBufferPtr>=dataBuffer; dataBufferPtr--) {
        value*=256;
        value+=*(unsigned char*)dataBufferPtr;
    }
    
    return value&0x7FFFFFFFFFFFFFFF;
}

static void UInt8ToHexCharPair(u_int8_t value, char* hexChars)
{
    const char hexCharMap[]="0123456789ABCDEF";
    
    hexChars[0]=hexCharMap[value>>4];
    hexChars[1]=hexCharMap[value&15];
}

static char* UnsignedToSmallEndianHex(long long value, size_t bytes, char* string) // string must be at least 2*bytes+1 long
{
    size_t byte;
    
    for (byte=0; byte<bytes; byte++) {
        UInt8ToHexCharPair(value&0xFF, string+2*byte);
        value/=256;
    }
    
    string[2*bytes]=0x00; // null terminator
    
    return string;
}

static char* BinaryToHex(const void* binary, const size_t bytes, char* hexString)
{
    size_t byte;
    
    for (byte=0; byte<bytes; byte++)
        UInt8ToHexCharPair(((u_int8_t*)binary)[byte], hexString+2*byte);
    
    hexString[2*bytes]=0x00; // null terminator

    return hexString;
}

static bool HexCharToUInt8(const char hexChar, u_int8_t* value)
{
    if ((hexChar>='0') && (hexChar<='9'))
        *value=hexChar-'0';
    else if ((hexChar>='a') && (hexChar<='f'))
        *value=hexChar-'a'+10;
    else if ((hexChar>='A') && (hexChar<='F'))
        *value=hexChar-'A'+10;
    else
        return FALSE;
    
    return TRUE;
}

static bool HexToBinary(const char* hexString, void* binary, const size_t bytes)
{
    size_t byte;
    u_int8_t valueHigh, valueLow;
    
    for (byte=0; byte<bytes; byte++)
        if (HexCharToUInt8(hexString[2*byte], &valueHigh) && HexCharToUInt8(hexString[2*byte+1], &valueLow))
            ((u_int8_t*)binary)[byte]=(valueHigh<<4)|valueLow;
        else
            return FALSE;
    
    return TRUE;
}

static void sha256(unsigned char hval[], const unsigned char data[], unsigned long len);

void CoinSparkCalcSHA256Hash(const unsigned char* input, const size_t inputLen, unsigned char hash[32])
{
    sha256(hash, input, inputLen);
}

size_t AllocRawScript(const char* scriptPubKey, const size_t scriptPubKeyLen, const bool scriptIsHex, const char** _scriptPubKeyRaw)
{
    size_t scriptPubKeyRawLen;
    char* scriptPubKeyRaw;
    
    if (scriptIsHex) {
        if ((scriptPubKeyLen%2)==0) {
            scriptPubKeyRawLen=scriptPubKeyLen/2;
            scriptPubKeyRaw=malloc(scriptPubKeyRawLen);
            
            if (HexToBinary(scriptPubKey, scriptPubKeyRaw, scriptPubKeyRawLen)) {
                *_scriptPubKeyRaw=scriptPubKeyRaw;
                return scriptPubKeyRawLen;
                
            } else {
                free(scriptPubKeyRaw);
                *_scriptPubKeyRaw=NULL;
                *scriptPubKeyRaw=0;
            }
        }
        
    } else {
        *_scriptPubKeyRaw=scriptPubKey;
        return scriptPubKeyLen;
    }
    
    return 0;
}

void FreeRawScript(const bool scriptIsHex, const char* scriptPubKeyRaw)
{
    if (scriptIsHex && scriptPubKeyRaw)
        free((void*)scriptPubKeyRaw);
}

static int Base58ToInteger(const char base58Character) // returns -1 if invalid
{
    if ( (base58Character<49) || (base58Character>122) )
        return -1;
    
    return base58Minus49ToInteger[base58Character-49];
}

static long long MantissaExponentToQty(int mantissa, int exponent)
{
    long long quantity;
    
    quantity=mantissa;
    
    for (; exponent>0; exponent--)
        quantity*=10;
    
    return quantity;
}

static long long QtyToMantissaExponent(long long quantity, int rounding, int mantissaMax, int exponentMax, int* mantissa, int* exponent)
{
    long long roundOffset;
    
    if (rounding<0)
        roundOffset=0;
    else if (rounding>0)
        roundOffset=9;
    else
        roundOffset=4;
    
    *exponent=0;
    
    while (quantity>mantissaMax) {
        quantity=(quantity+roundOffset)/10;
        (*exponent)++;
    }
    
    *mantissa=(int)quantity;
    *exponent=COINSPARK_MIN(*exponent, exponentMax);
    
    return MantissaExponentToQty(*mantissa, *exponent);
}

CoinSparkSatoshiQty GetMinFeeBasis(const CoinSparkSatoshiQty* outputsSatoshis, const bool* outputsRegular, const int countOutputs)
{
    CoinSparkSatoshiQty smallestOutputSatoshis;
    int outputIndex;
    
    smallestOutputSatoshis=COINSPARK_SATOSHI_QTY_MAX;
    
    for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
        if (outputsRegular[outputIndex])
            smallestOutputSatoshis=COINSPARK_MIN(smallestOutputSatoshis, outputsSatoshis[outputIndex]);
    
    return COINSPARK_MIN(COINSPARK_FEE_BASIS_MAX_SATOSHIS, smallestOutputSatoshis);
}

static int GetLastRegularOutput(const bool* outputsRegular, const int countOutputs)
{
    int outputIndex;
    
    for (outputIndex=countOutputs-1; outputIndex>=0; outputIndex--)
        if (outputsRegular[outputIndex])
            return outputIndex;
    
    return countOutputs; // indicates no regular ones were found
}

static int CountNonLastRegularOutputs(const bool* outputsRegular, const int countOutputs)
{
    int countRegularOutputs, outputIndex;
    
    countRegularOutputs=0;
    
    for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
        if (outputsRegular[outputIndex])
            countRegularOutputs++;
    
    return COINSPARK_MAX(countRegularOutputs-1, 0);
}

static void GetDefaultRouteMap(const CoinSparkTransfer* transfers, const int countTransfers,
                               const int countInputs, const int countOutputs, const bool* outputsRegular, int* inputDefaultOutput)
{
    int lastRegularOutput, transferIndex, inputIndex, lastInputIndex, outputIndex;
    
//  Default to last output for all inputs
    
    lastRegularOutput=GetLastRegularOutput(outputsRegular, countOutputs); // can be countOutputs if no regular ones found
    for (inputIndex=0; inputIndex<countInputs; inputIndex++)
        inputDefaultOutput[inputIndex]=lastRegularOutput;

//  Apply any default route transfers in reverse order (since early ones take precedence)
    
    for (transferIndex=countTransfers-1; transferIndex>=0; transferIndex--)
        if (transfers[transferIndex].assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) {
            outputIndex=transfers[transferIndex].outputs.first; // outputs.count is not relevant
            
            if ( (outputIndex>=0) && (outputIndex<countOutputs) ) {
                inputIndex=COINSPARK_MAX(transfers[transferIndex].inputs.first, 0);
                lastInputIndex=COINSPARK_MIN(inputIndex+transfers[transferIndex].inputs.count, countInputs)-1;
                
                for (; inputIndex<=lastInputIndex; inputIndex++)
                    inputDefaultOutput[inputIndex]=outputIndex;
            }
        }
}

static void GetPackingOptions(const CoinSparkIORange *previousRange, const CoinSparkIORange *range, const int countInputOutputs, PackingOptions packingOptions)
{
    bool firstZero, firstByte, first2Bytes, countOne, countByte;
    
    firstZero=(range->first==0);
    firstByte=(range->first<=COINSPARK_UNSIGNED_BYTE_MAX);
    first2Bytes=(range->first<=COINSPARK_UNSIGNED_2_BYTES_MAX);
    countOne=(range->count==1);
    countByte=(range->count<=COINSPARK_UNSIGNED_BYTE_MAX);
    
    if (previousRange) {
        packingOptions[_0P]=(range->first==previousRange->first) && (range->count==previousRange->count);
        packingOptions[_1S]=(range->first==(previousRange->first+previousRange->count)) && countOne;
        
    } else {
        packingOptions[_0P]=firstZero && countOne;
        packingOptions[_1S]=(range->first==1) && countOne;
    }
    
    packingOptions[_BYTE]=firstByte && countOne;
    packingOptions[_2_BYTES]=first2Bytes && countOne;
    packingOptions[_1_1_BYTES]=firstByte && countByte;
    packingOptions[_2_1_BYTES]=first2Bytes && countByte;
    packingOptions[_2_2_BYTES]=first2Bytes && (range->count<=COINSPARK_UNSIGNED_2_BYTES_MAX);
    packingOptions[_ALL]=firstZero && (range->count>=countInputOutputs);
}

static void PackingTypeToValues(const PackingType packingType, const CoinSparkIORange *previousRange, const int countInputOutputs, CoinSparkIORange *range)
{
    switch (packingType)
    {
        case _0P:
            if (previousRange) {
                range->first=previousRange->first;
                range->count=previousRange->count;
            } else {
                range->first=0;
                range->count=1;
            }
            break;
            
        case _1S:
            if (previousRange)
                range->first=previousRange->first+previousRange->count;
            else
                range->first=1;
            
            range->count=1;
            break;
            
        case _BYTE:
        case _2_BYTES:
            range->count=1;
            break;
            
        case _ALL:
            range->first=0;
            range->count=countInputOutputs;
            break;
            
        default: // to prevent compiler warning
            break;
    }
}

static void PackingToByteCounts(char packing, char packingExtend, PackingByteCounts* counts)
{
    
//  Set default values for bytes for all fields to zero
    
    counts->blockNumBytes=0;
    counts->txOffsetBytes=0;
    counts->txIDPrefixBytes=0;
    
    counts->firstInputBytes=0;
    counts->countInputsBytes=0;
    counts->firstOutputBytes=0;
    counts->countOutputsBytes=0;
    
    counts->quantityBytes=0;
    
//  Packing for genesis reference
    
    switch (packing & COINSPARK_PACKING_GENESIS_MASK)
    {
        case COINSPARK_PACKING_GENESIS_3_3_BYTES:
            counts->blockNumBytes=3;
            counts->txOffsetBytes=3;
            counts->txIDPrefixBytes=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
            break;
            
        case COINSPARK_PACKING_GENESIS_3_4_BYTES:
            counts->blockNumBytes=3;
            counts->txOffsetBytes=4;
            counts->txIDPrefixBytes=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
            break;
            
        case COINSPARK_PACKING_GENESIS_4_4_BYTES:
            counts->blockNumBytes=4;
            counts->txOffsetBytes=4;
            counts->txIDPrefixBytes=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
            break;
    }
    
//  Packing for input and output indices (relevant for extended indices only)
    
    if ((packing & COINSPARK_PACKING_INDICES_MASK) == COINSPARK_PACKING_INDICES_EXTEND) {
    
    //  Input indices
        
        switch ((packingExtend >> COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK)
        {
            case COINSPARK_PACKING_EXTEND_BYTE:
                counts->firstInputBytes=1;
                break;
                
            case COINSPARK_PACKING_EXTEND_2_BYTES:
                counts->firstInputBytes=2;
                break;
                
            case COINSPARK_PACKING_EXTEND_1_1_BYTES:
                counts->firstInputBytes=1;
                counts->countInputsBytes=1;
                break;
                
            case COINSPARK_PACKING_EXTEND_2_1_BYTES:
                counts->firstInputBytes=2;
                counts->countInputsBytes=1;
                break;
                
            case COINSPARK_PACKING_EXTEND_2_2_BYTES:
                counts->firstInputBytes=2;
                counts->countInputsBytes=2;
                break;
        }
        
    //  Output indices
        
        switch ((packingExtend >> COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK)
        {
            case COINSPARK_PACKING_EXTEND_BYTE:
                counts->firstOutputBytes=1;
                break;
                
            case COINSPARK_PACKING_EXTEND_2_BYTES:
                counts->firstOutputBytes=2;
                break;
                
            case COINSPARK_PACKING_EXTEND_1_1_BYTES:
                counts->firstOutputBytes=1;
                counts->countOutputsBytes=1;
                break;
                
            case COINSPARK_PACKING_EXTEND_2_1_BYTES:
                counts->firstOutputBytes=2;
                counts->countOutputsBytes=1;
                break;
                
            case COINSPARK_PACKING_EXTEND_2_2_BYTES:
                counts->firstOutputBytes=2;
                counts->countOutputsBytes=2;
                break;
        }
        
    }
    
//  Packing for quantity
    
    switch (packing & COINSPARK_PACKING_QUANTITY_MASK)
    {
        case COINSPARK_PACKING_QUANTITY_1_BYTE:
            counts->quantityBytes=1;
            break;
            
        case COINSPARK_PACKING_QUANTITY_2_BYTES:
            counts->quantityBytes=2;
            break;
            
        case COINSPARK_PACKING_QUANTITY_3_BYTES:
            counts->quantityBytes=3;
            break;
            
        case COINSPARK_PACKING_QUANTITY_4_BYTES:
            counts->quantityBytes=4;
            break;
            
        case COINSPARK_PACKING_QUANTITY_6_BYTES:
            counts->quantityBytes=6;
            break;
            
        case COINSPARK_PACKING_QUANTITY_FLOAT:
            counts->quantityBytes=COINSPARK_TRANSFER_QTY_FLOAT_LENGTH;
            break;
    }
}

static bool EncodePackingExtend(const PackingOptions packingOptions, char *packingExtend)
{
    PackingType option;
    
    for (option=firstPackingType; option<countPackingTypes; option++)
        if (packingOptions[option]) {
            *packingExtend=packingExtendMap[option];
            return TRUE;
        }
    
    return FALSE;
}

static bool DecodePackingExtend(const char packingExtend, PackingType *packingType)
{
    PackingType option;
    
    for (option=firstPackingType; option<countPackingTypes; option++)
        if (packingExtend==packingExtendMap[option]) {
            *packingType=option;
            return TRUE;
        }
    
    return FALSE;
}

static bool LocateMetadataRange(const char** _metadataPtr, const char** _metadataEnd, char desiredPrefix)
{
    char foundPrefix;
    const char *metadataPtr, *metadataEnd;
    
    metadataPtr=*_metadataPtr;
    metadataEnd=*_metadataEnd;
    
    if ( (metadataPtr+COINSPARK_METADATA_IDENTIFIER_LEN+1) > metadataEnd ) // check for 4 bytes at least
        return FALSE;
    
    if (memcmp(metadataPtr, COINSPARK_METADATA_IDENTIFIER, COINSPARK_METADATA_IDENTIFIER_LEN)) // check it starts 'SPK'
        return FALSE;
    
    metadataPtr+=COINSPARK_METADATA_IDENTIFIER_LEN; // skip past 'SPK'
    
    while (metadataPtr<metadataEnd) {
        foundPrefix=*metadataPtr++; // read the next prefix
        
        if (desiredPrefix ? (foundPrefix==desiredPrefix) : (foundPrefix>COINSPARK_LENGTH_PREFIX_MAX)) {
            // it's our data from here to the end (if desiredPrefix is 0, it matches the last one whichever it is)
            *_metadataPtr=metadataPtr;
            return TRUE;
        }
        
        if (foundPrefix>COINSPARK_LENGTH_PREFIX_MAX) // it's some other type of data from here to end
            return FALSE;
        
        // if we get here it means we found a length byte
        
        if (foundPrefix>(metadataEnd-metadataPtr)) // something went wrong - length indicated is longer than that available
            return FALSE;
        
        if (metadataPtr>=metadataEnd) // something went wrong - that was the end of the input data
            return FALSE;
        
        if (*metadataPtr==desiredPrefix) { // it's the length of our part
            *_metadataPtr=metadataPtr+1;
            *_metadataEnd=metadataPtr+foundPrefix;
            return TRUE;
            
        } else
            metadataPtr+=foundPrefix; // skip over this many bytes
    }
    
    return FALSE;
}

static size_t ShrinkLowerDomainName(const char* fullDomainName, size_t fullDomainNameLen, char* shortDomainName, size_t shortDomainNameMaxLen, char* packing)
{
    char sourceDomainName[256];
    int charIndex, bestPrefixLen, prefixIndex, prefixLen, bestPrefixIndex, bestSuffixLen, suffixIndex, suffixLen, bestSuffixIndex;
    size_t sourceDomainLen;

//  Check we have things in range
    
    if (fullDomainNameLen>=sizeof(sourceDomainName)) // >= because of null terminator
        return 0;
    
    if (fullDomainNameLen<=0)
        return 0; // nothing there
    
//  Convert to lower case and C-terminated string
    
    sourceDomainLen=fullDomainNameLen;

    for (charIndex=0; charIndex<sourceDomainLen; charIndex++)
        sourceDomainName[charIndex]=tolower(fullDomainName[charIndex]);
    
    sourceDomainName[sourceDomainLen]=0;

//  Search for prefixes
    
    bestPrefixLen=-1;
    for (prefixIndex=0; prefixIndex<(sizeof(domainNamePrefixes)/sizeof(*domainNamePrefixes)); prefixIndex++) {
        prefixLen=(int)strlen(domainNamePrefixes[prefixIndex]);
        
        if ( (prefixLen>bestPrefixLen) && (strncmp(sourceDomainName, domainNamePrefixes[prefixIndex], prefixLen)==0) ) {
            bestPrefixIndex=prefixIndex;
            bestPrefixLen=prefixLen;
        }
    }
    
    sourceDomainLen-=bestPrefixLen;
    memmove(sourceDomainName, sourceDomainName+bestPrefixLen, sourceDomainLen+1); // includes null terminator
    
//  Search for suffixes
    
    bestSuffixLen=-1;
    for (suffixIndex=0; suffixIndex<(sizeof(domainNameSuffixes)/sizeof(*domainNameSuffixes)); suffixIndex++) {
        suffixLen=(int)strlen(domainNameSuffixes[suffixIndex]);
        
        if ( (suffixLen>bestSuffixLen) && (strncmp(sourceDomainName+sourceDomainLen-suffixLen, domainNameSuffixes[suffixIndex], suffixLen)==0) ) {
            bestSuffixIndex=suffixIndex;
            bestSuffixLen=suffixLen;
        }
    }
    
    sourceDomainLen-=bestSuffixLen;
    sourceDomainName[sourceDomainLen]=0x00;
    
//  Output and return
    
    if (sourceDomainLen>=shortDomainNameMaxLen)
        return 0;
    
    strcpy(shortDomainName, sourceDomainName);
    *packing=((bestPrefixIndex<<COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT)&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)|
        (bestSuffixIndex&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK);
 
    return sourceDomainLen;
}

static size_t ExpandDomainName(const char* shortDomainName, size_t shortDomainNameLen, const char packing, char* fullDomainName, size_t fullDomainNameMaxLen)
{
    char destDomainName[256];
    int prefixIndex, prefixLen, suffixIndex;
    size_t destDomainLen;
    
    if (shortDomainNameLen>=(sizeof(destDomainName)-12))
        return 0; // too long to safely expand

//  Convert to C-terminated string
    
    destDomainLen=shortDomainNameLen;
    memmove(destDomainName, shortDomainName, destDomainLen);
    destDomainName[destDomainLen]=0;
    
//  Prepend prefix
    
    prefixIndex=(packing&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)>>COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT;
    if (prefixIndex>=(sizeof(domainNamePrefixes)/sizeof(*domainNamePrefixes)))
        return 0; // out of range
    
    prefixLen=(int)strlen(domainNamePrefixes[prefixIndex]);
    memmove(destDomainName+prefixLen, destDomainName, destDomainLen+1);
    memcpy(destDomainName, domainNamePrefixes[prefixIndex], prefixLen);
    destDomainLen+=prefixLen;
    
//  Append suffix
    
    suffixIndex=packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK;
    if (suffixIndex>=(sizeof(domainNameSuffixes)/sizeof(*domainNameSuffixes)))
        return 0; // out of range

    strcpy(destDomainName+destDomainLen, domainNameSuffixes[suffixIndex]);
    destDomainLen+=strlen(domainNameSuffixes[suffixIndex]);
    
//  Output and return
    
    if (destDomainLen>fullDomainNameMaxLen)
        return 0;
    
    strcpy(fullDomainName, destDomainName);
    
    return destDomainLen;
}

static bool ReadIPv4Address(const char* string, u_int8_t* octets)
{
    int octetNum, octetValue;
    char stringChar;
    
    for (octetNum=0; octetNum<4; octetNum++) {
        octetValue=0;
        
        while (TRUE) {
            stringChar=*string++;
            
            if ((stringChar>='0') && (stringChar<='9')) {
                octetValue=octetValue*10+(stringChar-'0');
                if (octetValue>255)
                    return FALSE;
            
            } else if ((stringChar=='.') || (stringChar==0x00)) {
                break;
                
            } else
                return FALSE;
        }

        octets[octetNum]=octetValue;
        if (stringChar != ((octetNum==3) ? 0x00 : '.'))
            return FALSE;
    }
    
    return TRUE;
}

static size_t EncodeDomainPathTriplets(const char* string, const size_t stringLen, char* _metadataPtr, const char* metadataEnd)
{
    int stringPos, stringTriplet, encodeValue;
    char *metadataPtr, *foundCharPtr;
    
    metadataPtr=_metadataPtr;
    
    stringTriplet=0;
    for (stringPos=0; stringPos<stringLen; stringPos++) {
        foundCharPtr=strchr(domainPathChars, tolower(string[stringPos]));
        if (foundCharPtr==NULL)
            goto cannotEncodeTriplets; // invalid character found
        
        encodeValue=(int)(foundCharPtr-domainPathChars);
        
        switch (stringPos%3)
        {
            case 0:
                stringTriplet=encodeValue;
                break;
                
            case 1:
                stringTriplet+=encodeValue*COINSPARK_DOMAIN_PATH_ENCODE_BASE;
                break;
                
            case 2:
                stringTriplet+=encodeValue*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE;
                break;
        }
        
        if ( ((stringPos%3)==2) || (stringPos==(stringLen-1)) ) { // write out 2 bytes if we've collected 3 chars, or if we're finishing
            if ((metadataPtr+2)<=metadataEnd) {
                if (!WriteSmallEndianUnsigned(stringTriplet, metadataPtr, 2))
                    goto cannotEncodeTriplets;
                
                metadataPtr+=2;
            } else
                goto cannotEncodeTriplets;
        }
    }
    
    return metadataPtr-_metadataPtr;
    
    cannotEncodeTriplets:
    return 0;
}

static size_t DecodeDomainPathTriplets(const char* _metadataPtr, const char* metadataEnd, char* string, size_t stringMaxLen, int parts)
{
    const char *metadataPtr;
    int stringPos, stringTriplet, decodeValue;
    char decodeChar;
    
    metadataPtr=_metadataPtr;
    stringPos=0;
    
    while (parts>0) {
        if ((stringPos+1)>=stringMaxLen)
            goto cannotDecodeTriplets; // ran out of buffer space
            
        if ((stringPos%3)==0) {
            if ((metadataPtr+2)<=metadataEnd) {
                stringTriplet=(int)ReadSmallEndianUnsigned(metadataPtr, 2);
                if (stringTriplet>=(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE))
                    goto cannotDecodeTriplets; // invalid value
                
                metadataPtr+=2;
                
            } else
                goto cannotDecodeTriplets; // ran out of metadata
        }
        
        switch (stringPos%3)
        {
            case 0:
                decodeValue=stringTriplet%COINSPARK_DOMAIN_PATH_ENCODE_BASE;
                break;
                
            case 1:
                decodeValue=(stringTriplet/COINSPARK_DOMAIN_PATH_ENCODE_BASE)%COINSPARK_DOMAIN_PATH_ENCODE_BASE;
                break;
                
            case 2:
                decodeValue=stringTriplet/(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE);
                break;
        }
        
        decodeChar=domainPathChars[decodeValue];
        string[stringPos++]=decodeChar;
       
        if ((decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR) || (decodeChar==COINSPARK_DOMAIN_PATH_FALSE_END_CHAR))
            parts--;
    }
    
    string[stringPos]=0x00; // null terminator

    return metadataPtr-_metadataPtr;
    
    cannotDecodeTriplets:
    return 0;
}

static size_t EncodeDomainAndOrPath(const char* domainName, bool useHttps, const char* pagePath, bool usePrefix,
                                      char* _metadataPtr, const char* metadataEnd)
{
    size_t encodeStringLen, pagePathLen, encodeLen;
    char *metadataPtr, packing, encodeString[256];
    u_int8_t octets[4];
    
    metadataPtr=_metadataPtr;
    encodeStringLen=0;

//  Domain name
    
    if (domainName) {
        if (ReadIPv4Address(domainName, octets)) { // special space-saving encoding for IPv4 addresses
            
            if ((metadataPtr+5)<=metadataEnd) {
                *metadataPtr++=COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4+(useHttps ? COINSPARK_DOMAIN_PACKING_IPv4_HTTPS : 0);
                
                ((u_int8_t*)metadataPtr)[0]=octets[0];
                ((u_int8_t*)metadataPtr)[1]=octets[1];
                ((u_int8_t*)metadataPtr)[2]=octets[2];
                ((u_int8_t*)metadataPtr)[3]=octets[3];
                
                metadataPtr+=4;
                
            } else
                goto cannotEncodeDomainAndPath;
            
        } else { // otherwise shrink the domain name and prepare it for encoding
            
            encodeStringLen=ShrinkLowerDomainName(domainName, strlen(domainName), encodeString, sizeof(encodeString), &packing);
            if (!encodeStringLen)
                goto cannotEncodeDomainAndPath;
            
            if (metadataPtr<metadataEnd)
                *metadataPtr++=packing;
            else
                goto cannotEncodeDomainAndPath;
            
            encodeString[encodeStringLen++]=useHttps ? COINSPARK_DOMAIN_PATH_TRUE_END_CHAR : COINSPARK_DOMAIN_PATH_FALSE_END_CHAR;
        }
    }
    
//  Page path
    
    if (pagePath) {
        pagePathLen=strlen(pagePath);
        if ((encodeStringLen+pagePathLen+2)>sizeof(encodeString)) // check sufficient space in local buffer
            goto cannotEncodeDomainAndPath;
        
        memcpy(encodeString+encodeStringLen, pagePath, pagePathLen);
        encodeStringLen+=pagePathLen;
        encodeString[encodeStringLen++]=usePrefix ? COINSPARK_DOMAIN_PATH_TRUE_END_CHAR : COINSPARK_DOMAIN_PATH_FALSE_END_CHAR;
    }
    
//  Encode whatever is required as triplets
    
    if (encodeStringLen) {
        encodeLen=EncodeDomainPathTriplets(encodeString, encodeStringLen, metadataPtr, metadataEnd);
        if (!encodeLen)
            goto cannotEncodeDomainAndPath;
        
        metadataPtr+=encodeLen;
    }
    
    return metadataPtr-_metadataPtr;
    
    cannotEncodeDomainAndPath:
    return 0;
}

static size_t DecodeDomainAndOrPath(const char* _metadataPtr, const char* metadataEnd, char* domainName, size_t domainNameMaxLen, bool* useHttps, char* pagePath, size_t pagePathMaxLen, bool* usePrefix)
{
    const char *metadataPtr;
    size_t decodedLen, ipAddressLen, metadataLen, decodePathLen, prevDecodedLen;
    bool isIpAddress;
    char packing, ipAddress[16], decodeString[256], decodeChar;
    u_int8_t octets[4];
    int metadataParts;

    metadataPtr=_metadataPtr;
    metadataParts=0;

//  Domain name
    
    if (domainName) {
 
    //  Get packing byte
        
        if (metadataPtr<metadataEnd)
            packing=*metadataPtr++;
        else
            goto cannotDecodeDomainAndPath;

    //  Extract IP address if present
        
        isIpAddress=((packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK)==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4);
        
        if (isIpAddress) {
            *useHttps=(packing&COINSPARK_DOMAIN_PACKING_IPv4_HTTPS) ? TRUE : FALSE;
            
            if ((metadataPtr+4)<=metadataEnd) {
                octets[0]=((u_int8_t*)metadataPtr)[0];
                octets[1]=((u_int8_t*)metadataPtr)[1];
                octets[2]=((u_int8_t*)metadataPtr)[2];
                octets[3]=((u_int8_t*)metadataPtr)[3];
                
                metadataPtr+=4;
            
            } else
                goto cannotDecodeDomainAndPath;
            
            sprintf(ipAddress, "%u.%u.%u.%u", octets[0], octets[1], octets[2], octets[3]);
            ipAddressLen=strlen(ipAddress);
            
            if (ipAddressLen>=domainNameMaxLen) // allow for null terminator
                goto cannotDecodeDomainAndPath;
            
            strcpy(domainName, ipAddress);
        
        } else
            metadataParts++;
    }
    
//  Convert remaining metadata to string
    
    if (pagePath)
        metadataParts++;
    
    if (metadataParts>0) {
        metadataLen=DecodeDomainPathTriplets(metadataPtr, metadataEnd, decodeString, sizeof(decodeString), metadataParts);
        if (!metadataLen)
            goto cannotDecodeDomainAndPath;

        metadataPtr+=metadataLen;
        decodedLen=0;
        
    //  Extract domain name if IP address was not present
        
        if (domainName && !isIpAddress)
            while (TRUE) {
                if (decodedLen>=sizeof(decodeString))
                    goto cannotDecodeDomainAndPath; // should never happen
                
                decodeChar=decodeString[decodedLen++];

                if ((decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR) || (decodeChar==COINSPARK_DOMAIN_PATH_FALSE_END_CHAR)) {
                    if (!ExpandDomainName(decodeString, decodedLen-1, packing, domainName, domainNameMaxLen))
                        goto cannotDecodeDomainAndPath;
                    
                    *useHttps=(decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR);
                    break;
                }
            }
        
    //  Extract page path
        
        prevDecodedLen=decodedLen;
        
        if (pagePath)
            while (TRUE) {
                if (decodedLen>=sizeof(decodeString))
                    goto cannotDecodeDomainAndPath; // should never happen

                decodeChar=decodeString[decodedLen++];
                
                if ((decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR) || (decodeChar==COINSPARK_DOMAIN_PATH_FALSE_END_CHAR)) {
                    decodePathLen=decodedLen-1-prevDecodedLen;
                    if (decodePathLen>=pagePathMaxLen)
                        goto cannotDecodeDomainAndPath;
                    
                    memcpy(pagePath, decodeString+prevDecodedLen, decodePathLen);
                    pagePath[decodePathLen]=0x00;
                    *usePrefix=(decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR);
                    break;
                }
            }
    }
    
//  Finish and return
    
    return metadataPtr-_metadataPtr;
    
    cannotDecodeDomainAndPath:
    return 0;
}

static int CompareAssetRefs(const CoinSparkAssetRef* assetRef1, const CoinSparkAssetRef* assetRef2)
{
    // -1 if assetRef1<assetRef2, 1 if assetRef2>assetRef1, 0 otherwise

    if (assetRef1->blockNum!=assetRef2->blockNum)
        return (assetRef1->blockNum<assetRef2->blockNum) ? -1 : 1;
    else if (assetRef1->blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) // in this case don't compare other fields
        return 0;
    else if (assetRef1->txOffset!=assetRef2->txOffset)
        return (assetRef1->txOffset<assetRef2->txOffset) ? -1 : 1;
    else
        return memcmp(assetRef1->txIDPrefix, assetRef2->txIDPrefix, COINSPARK_ASSETREF_TXID_PREFIX_LEN);
}

static void TransfersGroupOrdering(const CoinSparkTransfer* transfers, int* ordering, int countTransfers)
{
    bool *transferUsed;
    int orderIndex, transferIndex, bestTransferIndex, transferScore, bestTransferScore;
    
    transferUsed=(bool*)malloc(countTransfers*sizeof(*transferUsed));
    
    for (transferIndex=0; transferIndex<countTransfers; transferIndex++)
        transferUsed[transferIndex]=FALSE;
    
    for (orderIndex=0; orderIndex<countTransfers; orderIndex++) {
        bestTransferScore=0;
        bestTransferIndex=-1;
        
        for (transferIndex=0; transferIndex<countTransfers; transferIndex++)
            if (!transferUsed[transferIndex]) {
                if (transfers[transferIndex].assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
                    transferScore=3; // top priority to default routes, which must be first in the encoded list
                else if ((orderIndex>0) && CoinSparkAssetRefMatch(&transfers[ordering[orderIndex-1]].assetRef, &transfers[transferIndex].assetRef))
                    transferScore=2; // then next best is one which has same asset reference as previous
                else
                    transferScore=1; // otherwise any will do
        
                if (transferScore>bestTransferScore) { // if it's clearly the best, take it
                    bestTransferScore=transferScore;
                    bestTransferIndex=transferIndex;
                
                } else if (transferScore==bestTransferScore) // otherwise give priority to "lower" asset references
                    if (CompareAssetRefs(&transfers[transferIndex].assetRef, &transfers[bestTransferIndex].assetRef)<0)
                        bestTransferIndex=transferIndex;
            }
        
        ordering[orderIndex]=bestTransferIndex;
        transferUsed[bestTransferIndex]=TRUE;
    }
    
    free(transferUsed);
}

// Public functions

size_t CoinSparkScriptToMetadata(const char* scriptPubKey, const size_t scriptPubKeyLen, const bool scriptIsHex,
                                 char* metadata, const size_t metadataMaxLen)
{
    size_t scriptPubKeyRawLen, metadataLen;
    const char *scriptPubKeyRaw;
    
    scriptPubKeyRawLen=AllocRawScript(scriptPubKey, scriptPubKeyLen, scriptIsHex, &scriptPubKeyRaw);
    metadataLen=scriptPubKeyRawLen-2;
    
    if ( (scriptPubKeyRawLen>2) && (scriptPubKeyRaw[0]==0x6a) && (scriptPubKeyRaw[1]>0) && (scriptPubKeyRaw[1]<=75) && (scriptPubKeyRaw[1]==metadataLen))
        memcpy(metadata, scriptPubKeyRaw+2, metadataLen);
    else
        metadataLen=0;
    
    FreeRawScript(scriptIsHex, scriptPubKeyRaw);
   
    return metadataLen;
}

size_t CoinSparkScriptsToMetadata(const char* scriptPubKeys[], const size_t scriptPubKeyLens[], const bool scriptsAreHex,
                                  const int countScripts, char* metadata, const size_t metadataMaxLen)
{
    int scriptIndex;
    
    for (scriptIndex=0; scriptIndex<countScripts; scriptIndex++)
        if (!CoinSparkScriptIsRegular(scriptPubKeys[scriptIndex], scriptPubKeyLens[scriptIndex], scriptsAreHex))
            return CoinSparkScriptToMetadata(scriptPubKeys[scriptIndex], scriptPubKeyLens[scriptIndex], scriptsAreHex, metadata, metadataMaxLen);
    
    return 0;
}

size_t CoinSparkMetadataToScript(const char* metadata, const size_t metadataLen,
                                 char* scriptPubKey, const size_t scriptPubKeyMaxLen, const bool toHexScript)
{
    size_t scriptRawLen;
    
    if (metadataLen<=75) {
        scriptRawLen=metadataLen+2;
        
        if (toHexScript) {
            if (scriptPubKeyMaxLen>=(2*scriptRawLen+1)) { // include null terminator for hex string
                UInt8ToHexCharPair(0x6a, scriptPubKey);
                UInt8ToHexCharPair(metadataLen, scriptPubKey+2);
                BinaryToHex(metadata, metadataLen, scriptPubKey+4);
                scriptPubKey[2*scriptRawLen]=0x00; // null terminator
                
                return 2*scriptRawLen;
            }
     
        } else {
            if (scriptPubKeyMaxLen>=scriptRawLen) {
                scriptPubKey[0]=0x6a;
                scriptPubKey[1]=metadataLen;
                memcpy(scriptPubKey+2, metadata, metadataLen);

                return scriptRawLen;
            }
        }
    }
    
    return 0;
}

size_t CoinSparkMetadataMaxAppendLen(char* metadata, const size_t metadataLen, const size_t metadataMaxLen)
{
    int appendLen;
    
    appendLen=(int)metadataMaxLen-((int)metadataLen+1-COINSPARK_METADATA_IDENTIFIER_LEN);

    return COINSPARK_MAX(appendLen, 0);
}

size_t CoinSparkMetadataAppend(char* metadata, const size_t metadataLen, const size_t metadataMaxLen, const char* appendMetadata, const size_t appendMetadataLen)
{
    size_t needLength, lastMetadataLen;
    char *lastMetadata, *lastMetadataEnd;
    
    lastMetadata=metadata;
    lastMetadataEnd=lastMetadata+metadataLen;
    
    if (!LocateMetadataRange((const char**)&lastMetadata, (const char**)&lastMetadataEnd, 0)) // check we can find last metadata
        return 0;
    
    if (appendMetadataLen<(COINSPARK_METADATA_IDENTIFIER_LEN+1)) // check there is enough to check the prefix
        return 0;
    
    if (memcmp(appendMetadata, COINSPARK_METADATA_IDENTIFIER, COINSPARK_METADATA_IDENTIFIER_LEN)) // then check the prefix
        return 0;
    
    // we don't check the character after the prefix in appendMetadata because it could itself be composite
               
    needLength=metadataLen+appendMetadataLen-COINSPARK_METADATA_IDENTIFIER_LEN+1; // check there is enough spae
    if (metadataMaxLen<needLength)
        return 0;
    
    lastMetadataLen=lastMetadataEnd-lastMetadata+1; // include prefix
    memmove(lastMetadata, lastMetadata-1, lastMetadataLen);
    lastMetadata[-1]=lastMetadataLen;
    
    memcpy(lastMetadata+lastMetadataLen, appendMetadata+COINSPARK_METADATA_IDENTIFIER_LEN, appendMetadataLen-COINSPARK_METADATA_IDENTIFIER_LEN);
    
    return needLength;
}

bool CoinSparkScriptIsRegular(const char* scriptPubKey, const size_t scriptPubKeyLen, const bool scriptIsHex)
{
    u_int8_t firstValue;
    
    if (scriptIsHex)
        return (scriptPubKeyLen<2) || (!HexToBinary(scriptPubKey, &firstValue, 1)) || (firstValue!=0x6a);
    else
        return (scriptPubKeyLen<1) || (scriptPubKey[0]!=0x6a);
}

void CoinSparkAddressClear(CoinSparkAddress* address)
{
    address->bitcoinAddress[0]=0x00;
    address->addressFlags=0;
    address->paymentRef=0;
}

bool CoinSparkAddressToString(const CoinSparkAddress* address, char* string, const size_t stringMaxLen)
{
    typedef struct {
        CoinSparkAddressFlags flag;
        char* string;
    } FlagToString;

    char buffer[1024], *bufferPtr;
    size_t bufferLength, copyLength;
    int flagIndex;
    bool flagOutput;

    FlagToString flagsToStrings[]={
        { COINSPARK_ADDRESS_FLAG_ASSETS, "assets" },
        { COINSPARK_ADDRESS_FLAG_PAYMENT_REFS, "payment references" }
    };
    
    bufferPtr=buffer;
    
    bufferPtr+=sprintf(bufferPtr, "COINSPARK ADDRESS\n");
    bufferPtr+=sprintf(bufferPtr, "  Bitcoin address: %s\n", address->bitcoinAddress);
    bufferPtr+=sprintf(bufferPtr, "    Address flags: %d", address->addressFlags);
    
    flagOutput=FALSE;

    for (flagIndex=0; flagIndex<(sizeof(flagsToStrings)/sizeof(*flagsToStrings)); flagIndex++)
        if (address->addressFlags & flagsToStrings[flagIndex].flag) {
            bufferPtr+=sprintf(bufferPtr, "%s%s", flagOutput ? ", " : " [", flagsToStrings[flagIndex].string);
            flagOutput=TRUE;
        }

    bufferPtr+=sprintf(bufferPtr, "%s\n", flagOutput ? "]" : "");
    
    bufferPtr+=sprintf(bufferPtr, "Payment reference: %lld\n", (long long)address->paymentRef);
    bufferPtr+=sprintf(bufferPtr, "END COINSPARK ADDRESS\n\n");

    bufferLength=bufferPtr-buffer;
    copyLength=COINSPARK_MIN(bufferLength, stringMaxLen-1);
    memcpy(string, buffer, copyLength);
    string[copyLength]=0x00;
    
    return (copyLength==bufferLength);
}

bool CoinSparkAddressIsValid(const CoinSparkAddress* address)
{
    if (strlen(address->bitcoinAddress)==0)
        return FALSE;

    if ((address->addressFlags&COINSPARK_ADDRESS_FLAG_MASK) != address->addressFlags)
        return FALSE;
    
    return CoinSparkPaymentRefIsValid(address->paymentRef);
}

bool CoinSparkAddressMatch(const CoinSparkAddress* address1, const CoinSparkAddress* address2)
{
    return (!strcmp(address1->bitcoinAddress, address2->bitcoinAddress)) &&
    (address1->addressFlags==address2->addressFlags) && (address1->paymentRef==address2->paymentRef);
}

size_t CoinSparkAddressEncode(const CoinSparkAddress* address, char* string, const size_t stringMaxLen)
{
    size_t bitcoinAddressLen, stringLen, halfLength;
    int charIndex, charValue, addressFlagChars, paymentRefChars, extraDataChars;
    CoinSparkAddressFlags testAddressFlags;
    CoinSparkPaymentRef testPaymentRef;
    char stringBase58[1024];
    
    if (!CoinSparkAddressIsValid(address))
        goto cannotEncodeAddress;
    
//  Build up extra data for address flags
    
    addressFlagChars=0;
    testAddressFlags=address->addressFlags;
    
    while (testAddressFlags>0) {
        stringBase58[2+addressFlagChars]=testAddressFlags%58;
        testAddressFlags/=58; // keep as integer
        addressFlagChars++;
    }
    
//  Build up extra data for payment reference
    
    paymentRefChars=0;
    testPaymentRef=address->paymentRef;
    
    while (testPaymentRef>0) {
        stringBase58[2+addressFlagChars+paymentRefChars]=testPaymentRef%58;
        testPaymentRef/=58; // keep as integer
        paymentRefChars++;
    }

//  Calculate/encode extra length and total length required
    
    extraDataChars=addressFlagChars+paymentRefChars;
    bitcoinAddressLen=strlen(address->bitcoinAddress);
    stringLen=bitcoinAddressLen+2+extraDataChars;

    if (stringMaxLen<=stringLen) // use <= because we will also add 0x00 C terminator byte
        return 0;
    
    if (stringLen>sizeof(stringBase58))
        return 0;

    stringBase58[1]=addressFlagChars*COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE+paymentRefChars;

//  Convert the bitcoin address
    
    for (charIndex=0; charIndex<bitcoinAddressLen; charIndex++) {
        charValue=Base58ToInteger(address->bitcoinAddress[charIndex]);
        if (charValue<0)
            return 0; // invalid base58 character

        charValue+=COINSPARK_ADDRESS_CHAR_INCREMENT;
        
        if (extraDataChars>0)
            charValue+=stringBase58[2+charIndex%extraDataChars];
        
        stringBase58[2+extraDataChars+charIndex]=charValue%58;
    }
    
//  Obfuscate first half of address using second half to prevent common prefixes
    
    halfLength=(stringLen+1)/2;
    for (charIndex=1; charIndex<halfLength; charIndex++) // exclude first character
        stringBase58[charIndex]=(stringBase58[charIndex]+stringBase58[stringLen-charIndex])%58;
    
//  Convert to base 58 and add prefix and terminator
    
    string[0]=COINSPARK_ADDRESS_PREFIX;
    for (charIndex=1; charIndex<stringLen; charIndex++)
        string[charIndex]=integerToBase58[stringBase58[charIndex]];
    string[stringLen]=0;
    
    return stringLen;
    
    cannotEncodeAddress:
    return 0;
}

bool CoinSparkAddressDecode(CoinSparkAddress* address, const char* string, const size_t stringLen)
{
    size_t bitcoinAddressLen, halfLength;
    int charIndex, charValue, addressFlagChars, paymentRefChars, extraDataChars;
    long long multiplier;
    char stringBase58[1024];
    
//  Check for basic validity
    
    if ( (stringLen<2) || (stringLen>sizeof(stringBase58)) )
        goto cannotDecodeAddress;
    
    if (string[0]!=COINSPARK_ADDRESS_PREFIX)
        goto cannotDecodeAddress;

//  Convert from base 58
    
    for (charIndex=1; charIndex<stringLen; charIndex++) { // exclude first character
        charValue=Base58ToInteger(string[charIndex]);
        if (charValue<0)
            goto cannotDecodeAddress;
        stringBase58[charIndex]=charValue;
    }
    
//  De-obfuscate first half of address using second half
    
    halfLength=(stringLen+1)/2;
    for (charIndex=1; charIndex<halfLength; charIndex++) // exclude first character
        stringBase58[charIndex]=(stringBase58[charIndex]+58-stringBase58[stringLen-charIndex])%58;
    
//  Get length of extra data
    
    charValue=stringBase58[1];
    addressFlagChars=charValue/COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE; // keep as integer
    paymentRefChars=charValue%COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE;
    extraDataChars=addressFlagChars+paymentRefChars;
    
    if (stringLen<(2+extraDataChars))
        goto cannotDecodeAddress;
    
//  Check we have sufficient length for the decoded address
    
    bitcoinAddressLen=stringLen-2-extraDataChars;
    if (sizeof(address->bitcoinAddress)<=bitcoinAddressLen) // use <= because we will also add 0x00 C terminator byte
        goto cannotDecodeAddress;
    
//  Read the extra data for address flags
    
    address->addressFlags=0;
    multiplier=1;
    
    for (charIndex=0; charIndex<addressFlagChars; charIndex++) {
        charValue=stringBase58[2+charIndex];
        address->addressFlags+=charValue*multiplier;
        multiplier*=58;
    }
    
//  Read the extra data for payment reference
    
    address->paymentRef=0;
    multiplier=1;
    
    for (charIndex=0; charIndex<paymentRefChars; charIndex++) {
        charValue=stringBase58[2+addressFlagChars+charIndex];
        address->paymentRef+=charValue*multiplier;
        multiplier*=58;
    }
    
//  Convert the bitcoin address
    
    for (charIndex=0; charIndex<bitcoinAddressLen; charIndex++) {
        charValue=stringBase58[2+extraDataChars+charIndex];
        charValue+=58*2-COINSPARK_ADDRESS_CHAR_INCREMENT; // avoid worrying about the result of modulo on negative numbers in any language
        
        if (extraDataChars>0)
            charValue-=stringBase58[2+charIndex%extraDataChars];
        
        address->bitcoinAddress[charIndex]=integerToBase58[charValue%58];
    }
    
    address->bitcoinAddress[bitcoinAddressLen]=0; // C terminator byte
    
    return CoinSparkAddressIsValid(address);
    
    cannotDecodeAddress:
    return FALSE;
}

void CoinSparkGenesisClear(CoinSparkGenesis *genesis)
{
    genesis->qtyMantissa=0;
    genesis->qtyExponent=0;
    genesis->chargeFlatMantissa=0;
    genesis->chargeFlatExponent=0;
    genesis->chargeBasisPoints=0;
    genesis->useHttps=FALSE;
    genesis->domainName[0]=0x00;
    genesis->usePrefix=TRUE;
    genesis->pagePath[0]=0x00;
    genesis->assetHashLen=0;
}

bool CoinSparkGenesisToString(const CoinSparkGenesis *genesis, char* string, const size_t stringMaxLen)
{
    char buffer[1024], hex[128], *bufferPtr;
    size_t bufferLength, copyLength, domainPathEncodeLen;
    int quantityEncoded, chargeFlatEncoded;
    CoinSparkAssetQty quantity, chargeFlat;
    char domainPathMetadata[64];
    
    bufferPtr=buffer;

    quantity=CoinSparkGenesisGetQty(genesis);
    quantityEncoded=(genesis->qtyExponent*COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+genesis->qtyMantissa)&COINSPARK_GENESIS_QTY_MASK;
    chargeFlat=CoinSparkGenesisGetChargeFlat(genesis);
    chargeFlatEncoded=genesis->chargeFlatExponent*COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+genesis->chargeFlatMantissa;
    domainPathEncodeLen=EncodeDomainAndOrPath(genesis->domainName, genesis->useHttps, genesis->pagePath, genesis->usePrefix,
                                            domainPathMetadata, domainPathMetadata+sizeof(domainPathMetadata));
    
    bufferPtr+=sprintf(bufferPtr, "COINSPARK GENESIS\n");
    bufferPtr+=sprintf(bufferPtr, "   Quantity mantissa: %d\n", genesis->qtyMantissa);
    bufferPtr+=sprintf(bufferPtr, "   Quantity exponent: %d\n", genesis->qtyExponent);
    bufferPtr+=sprintf(bufferPtr, "    Quantity encoded: %d (small endian hex %s)\n", quantityEncoded, UnsignedToSmallEndianHex(quantityEncoded, COINSPARK_GENESIS_QTY_FLAGS_LENGTH, hex));
    bufferPtr+=sprintf(bufferPtr, "      Quantity value: %lld\n", (long long)quantity);
    bufferPtr+=sprintf(bufferPtr, "Flat charge mantissa: %d\n", genesis->chargeFlatMantissa);
    bufferPtr+=sprintf(bufferPtr, "Flat charge exponent: %d\n", genesis->chargeFlatExponent);
    bufferPtr+=sprintf(bufferPtr, " Flat charge encoded: %d (small endian hex %s)\n", chargeFlatEncoded, UnsignedToSmallEndianHex(chargeFlatEncoded, COINSPARK_GENESIS_CHARGE_FLAT_LENGTH, hex));
    bufferPtr+=sprintf(bufferPtr, "   Flat charge value: %lld\n", (long long)chargeFlat);
    bufferPtr+=sprintf(bufferPtr, " Basis points charge: %d (hex %s)\n", genesis->chargeBasisPoints, UnsignedToSmallEndianHex(genesis->chargeBasisPoints, COINSPARK_GENESIS_CHARGE_BPS_LENGTH, hex));
    bufferPtr+=sprintf(bufferPtr, "           Asset URL: %s://%s/%s%s/ (length %zd+%zd encoded %s length %zd)\n",
        genesis->useHttps ? "https" : "http", genesis->domainName,
        genesis->usePrefix ? "coinspark/" : "", genesis->pagePath[0] ? genesis->pagePath : "[spent-txid]",
        strlen(genesis->domainName), strlen(genesis->pagePath),
        BinaryToHex(domainPathMetadata, domainPathEncodeLen, hex), domainPathEncodeLen);
    bufferPtr+=sprintf(bufferPtr, "          Asset hash: %s (length %zd)\n", BinaryToHex(genesis->assetHash, genesis->assetHashLen, hex), genesis->assetHashLen);
    bufferPtr+=sprintf(bufferPtr, "END COINSPARK GENESIS\n\n");
    
    bufferLength=bufferPtr-buffer;
    copyLength=COINSPARK_MIN(bufferLength, stringMaxLen-1);
    memcpy(string, buffer, copyLength);
    string[copyLength]=0x00;
    
    return (copyLength==bufferLength);
}

bool CoinSparkGenesisIsValid(const CoinSparkGenesis *genesis)
{
    if ( (genesis->qtyMantissa<COINSPARK_GENESIS_QTY_MANTISSA_MIN) || (genesis->qtyMantissa>COINSPARK_GENESIS_QTY_MANTISSA_MAX) )
        goto genesisIsInvalid;
    
    if ( (genesis->qtyExponent<COINSPARK_GENESIS_QTY_EXPONENT_MIN) || (genesis->qtyExponent>COINSPARK_GENESIS_QTY_EXPONENT_MAX) )
        goto genesisIsInvalid;
    
    if ( (genesis->chargeFlatExponent<COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN) || (genesis->chargeFlatExponent>COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) )
        goto genesisIsInvalid;
    
    if (genesis->chargeFlatMantissa<COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN)
        goto genesisIsInvalid;
    
    if (genesis->chargeFlatMantissa > ((genesis->chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) ? COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX : COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX))
        goto genesisIsInvalid;
    
    if ( (genesis->chargeBasisPoints<COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN) || (genesis->chargeBasisPoints>COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX) )
        goto genesisIsInvalid;
    
    if (strlen(genesis->domainName)>COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN)
        goto genesisIsInvalid;
    
    if (strlen(genesis->pagePath)>COINSPARK_GENESIS_PAGE_PATH_MAX_LEN)
        goto genesisIsInvalid;
    
    if ( (genesis->assetHashLen<COINSPARK_GENESIS_HASH_MIN_LEN) || (genesis->assetHashLen>COINSPARK_GENESIS_HASH_MAX_LEN) )
        goto genesisIsInvalid;
    
    return TRUE;
    
    genesisIsInvalid:
    return FALSE;
}

bool CoinSparkGenesisMatch(const CoinSparkGenesis* genesis1, const CoinSparkGenesis* genesis2, const bool strict)
{
    size_t hashCompareLen;
    bool floatQuantitiesMatch;
    
    hashCompareLen=COINSPARK_MIN(genesis1->assetHashLen, genesis2->assetHashLen);
    hashCompareLen=COINSPARK_MIN(hashCompareLen, COINSPARK_GENESIS_HASH_MAX_LEN);
    
    if (strict)
        floatQuantitiesMatch=(genesis1->qtyMantissa==genesis2->qtyMantissa) && (genesis1->qtyExponent==genesis2->qtyExponent)
        && (genesis1->chargeFlatMantissa==genesis2->chargeFlatMantissa) && (genesis1->chargeFlatExponent==genesis2->chargeFlatExponent);
    else
        floatQuantitiesMatch=(CoinSparkGenesisGetQty(genesis1)==CoinSparkGenesisGetQty(genesis2)) &&
        (CoinSparkGenesisGetChargeFlat(genesis1)==CoinSparkGenesisGetChargeFlat(genesis2));
    
    return
        floatQuantitiesMatch && (genesis1->chargeBasisPoints==genesis2->chargeBasisPoints) &&
        (genesis1->useHttps==genesis2->useHttps) &&
        (!strcasecmp(genesis1->domainName, genesis2->domainName)) &&
        (genesis1->usePrefix==genesis2->usePrefix) &&
        (!strcasecmp(genesis1->pagePath, genesis2->pagePath)) &&
        (!memcmp(&genesis1->assetHash, &genesis2->assetHash, hashCompareLen));
}

CoinSparkAssetQty CoinSparkGenesisGetQty(const CoinSparkGenesis *genesis)
{
    return MantissaExponentToQty(genesis->qtyMantissa, genesis->qtyExponent);
}

CoinSparkAssetQty CoinSparkGenesisSetQty(CoinSparkGenesis *genesis, const CoinSparkAssetQty desiredQty, const int rounding)
{
    int qtyMantissa, qtyExponent;
    
    QtyToMantissaExponent(desiredQty, rounding, COINSPARK_GENESIS_QTY_MANTISSA_MAX, COINSPARK_GENESIS_QTY_EXPONENT_MAX,
                          &qtyMantissa, &qtyExponent);
    
    genesis->qtyMantissa=qtyMantissa;
    genesis->qtyExponent=qtyExponent;
    
    return CoinSparkGenesisGetQty(genesis);
}

CoinSparkAssetQty CoinSparkGenesisGetChargeFlat(const CoinSparkGenesis *genesis)
{
    return MantissaExponentToQty(genesis->chargeFlatMantissa, genesis->chargeFlatExponent);
}

CoinSparkAssetQty CoinSparkGenesisSetChargeFlat(CoinSparkGenesis *genesis, CoinSparkAssetQty desiredChargeFlat, const int rounding)
{
    int chargeFlatMantissa, chargeFlatExponent;
    
    QtyToMantissaExponent(desiredChargeFlat, rounding, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX, COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX, &chargeFlatMantissa, &chargeFlatExponent);
    
    if (chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX)
        chargeFlatMantissa=COINSPARK_MIN(chargeFlatMantissa, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX);
    
    genesis->chargeFlatMantissa=chargeFlatMantissa;
    genesis->chargeFlatExponent=chargeFlatExponent;
    
    return CoinSparkGenesisGetChargeFlat(genesis);
}

CoinSparkAssetQty CoinSparkGenesisCalcCharge(const CoinSparkGenesis *genesis, CoinSparkAssetQty qtyGross)
{
    CoinSparkAssetQty charge;
    
    charge=CoinSparkGenesisGetChargeFlat(genesis)+(qtyGross*genesis->chargeBasisPoints+5000)/10000; // rounds to nearest
    
    return COINSPARK_MIN(qtyGross, charge); // can't charge more than the final amount
}

CoinSparkAssetQty CoinSparkGenesisCalcNet(const CoinSparkGenesis *genesis, CoinSparkAssetQty qtyGross)
{
    return qtyGross-CoinSparkGenesisCalcCharge(genesis, qtyGross);
}

CoinSparkAssetQty CoinSparkGenesisCalcGross(const CoinSparkGenesis *genesis, CoinSparkAssetQty qtyNet)
{
    CoinSparkAssetQty lowerGross;
    
    if (qtyNet<=0)
        return 0; // no point getting past charges if we end up with zero anyway
    
    lowerGross=((qtyNet+CoinSparkGenesisGetChargeFlat(genesis))*10000)/(10000-genesis->chargeBasisPoints); // divides rounding down
    
    return (CoinSparkGenesisCalcNet(genesis, lowerGross)>=qtyNet) ? lowerGross : (lowerGross+1);
}

size_t CoinSparkGenesisCalcHashLen(CoinSparkGenesis *genesis, const size_t metadataMaxLen)
{
    size_t domainPathLen;
    int assetHashLen;
    u_int8_t octets[4];
    char domainName[256], packing;
    
    assetHashLen=(int)metadataMaxLen-COINSPARK_METADATA_IDENTIFIER_LEN-1-COINSPARK_GENESIS_QTY_FLAGS_LENGTH;
    
    if (genesis->chargeFlatMantissa>0)
        assetHashLen-=COINSPARK_GENESIS_CHARGE_FLAT_LENGTH;
    
    if (genesis->chargeBasisPoints>0)
        assetHashLen-=COINSPARK_GENESIS_CHARGE_BPS_LENGTH;
    
    domainPathLen=strlen(genesis->pagePath)+1;
    
    if (ReadIPv4Address(genesis->domainName, octets))
        assetHashLen-=5; // packing and IP octets
    else {
        assetHashLen-=1; // packing
        domainPathLen+=ShrinkLowerDomainName(genesis->domainName, strlen(genesis->domainName), domainName, sizeof(domainName), &packing)+1;
    }
    
    assetHashLen-=2*((domainPathLen+2)/3); // uses integer arithmetic

    return COINSPARK_MIN(COINSPARK_MAX(assetHashLen, 0), COINSPARK_GENESIS_HASH_MAX_LEN);
}

size_t CoinSparkGenesisEncode(const CoinSparkGenesis *genesis, char* metadata, const size_t metadataMaxLen)
{
    char* metadataPtr, *metadataEnd;
    size_t encodeLen;
    int quantityEncoded, chargeEncoded;
    
    if (!CoinSparkGenesisIsValid(genesis))
        goto cannotEncodeGenesis;
    
    metadataPtr=metadata;
    metadataEnd=metadataPtr+metadataMaxLen;
    
//  4-character identifier
    
    if ((metadataPtr+COINSPARK_METADATA_IDENTIFIER_LEN+1)<=metadataEnd) {
        memcpy(metadataPtr, COINSPARK_METADATA_IDENTIFIER, COINSPARK_METADATA_IDENTIFIER_LEN);
        metadataPtr+=COINSPARK_METADATA_IDENTIFIER_LEN;
        *metadataPtr++=COINSPARK_GENESIS_PREFIX;
    } else
        goto cannotEncodeGenesis;
    
//  Quantity mantissa and exponent
    
    quantityEncoded=(genesis->qtyExponent*COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+genesis->qtyMantissa)&COINSPARK_GENESIS_QTY_MASK;
    if (genesis->chargeFlatMantissa>0)
        quantityEncoded|=COINSPARK_GENESIS_FLAG_CHARGE_FLAT;
    if (genesis->chargeBasisPoints>0)
        quantityEncoded|=COINSPARK_GENESIS_FLAG_CHARGE_BPS;
    
    if ((metadataPtr+COINSPARK_GENESIS_QTY_FLAGS_LENGTH)<=metadataEnd) {
        if (!WriteSmallEndianUnsigned(quantityEncoded, metadataPtr, COINSPARK_GENESIS_QTY_FLAGS_LENGTH))
            goto cannotEncodeGenesis;
        
        metadataPtr+=COINSPARK_GENESIS_QTY_FLAGS_LENGTH;
    } else
        goto cannotEncodeGenesis;
    
//  Charges - flat and basis points
    
    if (quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_FLAT) {
        chargeEncoded=genesis->chargeFlatExponent*COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+genesis->chargeFlatMantissa;
        
        if ((metadataPtr+COINSPARK_GENESIS_CHARGE_FLAT_LENGTH)<=metadataEnd) {
            if (!WriteSmallEndianUnsigned(chargeEncoded, metadataPtr, COINSPARK_GENESIS_CHARGE_FLAT_LENGTH))
                goto cannotEncodeGenesis;
            
            metadataPtr+=COINSPARK_GENESIS_CHARGE_FLAT_LENGTH;
            
        } else
            goto cannotEncodeGenesis;
    }
    
    if (quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_BPS) {
        if ((metadataPtr+COINSPARK_GENESIS_CHARGE_BPS_LENGTH)<=metadataEnd) {
            if (!WriteSmallEndianUnsigned(genesis->chargeBasisPoints, metadataPtr, COINSPARK_GENESIS_CHARGE_BPS_LENGTH))
                goto cannotEncodeGenesis;
            
            metadataPtr+=COINSPARK_GENESIS_CHARGE_BPS_LENGTH;
            
        } else
            goto cannotEncodeGenesis;
    }
    
//  Domain name and page path
    
    encodeLen=EncodeDomainAndOrPath(genesis->domainName, genesis->useHttps, genesis->pagePath, genesis->usePrefix, metadataPtr, metadataEnd);
    if (!encodeLen)
        goto cannotEncodeGenesis;
    
    metadataPtr+=encodeLen;
        
//  Asset hash
    
    if ((metadataPtr+genesis->assetHashLen)<=metadataEnd) {
        memcpy(metadataPtr, genesis->assetHash, genesis->assetHashLen);
        metadataPtr+=genesis->assetHashLen;
    } else
        goto cannotEncodeGenesis;
    
//  Return the number of bytes used
    
    return metadataPtr-metadata;
    
    cannotEncodeGenesis:
    return 0;
}

bool CoinSparkGenesisDecode(CoinSparkGenesis* genesis, const char* metadata, const size_t metadataLen)
{
    const char *metadataPtr, *metadataEnd;
    int quantityEncoded, chargeEncoded;
    size_t decodeLen;
    
    metadataPtr=metadata;
    metadataEnd=metadataPtr+metadataLen;
    
    if (!LocateMetadataRange(&metadataPtr, &metadataEnd, COINSPARK_GENESIS_PREFIX))
        goto cannotDecodeGenesis;
    
//  Quantity mantissa and exponent
    
    if ((metadataPtr+COINSPARK_GENESIS_QTY_FLAGS_LENGTH)<=metadataEnd) {
        quantityEncoded=(int)ReadSmallEndianUnsigned(metadataPtr, COINSPARK_GENESIS_QTY_FLAGS_LENGTH);
        metadataPtr+=COINSPARK_GENESIS_QTY_FLAGS_LENGTH;
        
        genesis->qtyMantissa=(quantityEncoded&COINSPARK_GENESIS_QTY_MASK)%COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE;
        genesis->qtyExponent=(quantityEncoded&COINSPARK_GENESIS_QTY_MASK)/COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE;
        
    } else
        goto cannotDecodeGenesis;
    
//  Charges - flat and basis points
    
    if (quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_FLAT) {
        if ((metadataPtr+COINSPARK_GENESIS_CHARGE_FLAT_LENGTH)<=metadataEnd) {
            chargeEncoded=(int)ReadSmallEndianUnsigned(metadataPtr, COINSPARK_GENESIS_CHARGE_FLAT_LENGTH);
            metadataPtr+=COINSPARK_GENESIS_CHARGE_FLAT_LENGTH;
            
            genesis->chargeFlatMantissa=chargeEncoded%COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE;
            genesis->chargeFlatExponent=chargeEncoded/COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE;
             
        } else
            goto cannotDecodeGenesis;
        
    } else {
        genesis->chargeFlatMantissa=0;
        genesis->chargeFlatExponent=0;
    }
    
    if (quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_BPS) {
        if ((metadataPtr+COINSPARK_GENESIS_CHARGE_BPS_LENGTH)<=metadataEnd) {
            genesis->chargeBasisPoints=(int16_t)ReadSmallEndianUnsigned(metadataPtr, COINSPARK_GENESIS_CHARGE_BPS_LENGTH);
            metadataPtr+=COINSPARK_GENESIS_CHARGE_BPS_LENGTH;
            
        } else
            goto cannotDecodeGenesis;
        
    } else
        genesis->chargeBasisPoints=0;
    
//  Domain name and page path
    
    decodeLen=DecodeDomainAndOrPath(metadataPtr, metadataEnd, genesis->domainName, sizeof(genesis->domainName),
                                    &genesis->useHttps, genesis->pagePath, sizeof(genesis->pagePath), &genesis->usePrefix);
    
    if (!decodeLen)
        goto cannotDecodeGenesis;
    
    metadataPtr+=decodeLen;
    
//  Asset hash
    
    genesis->assetHashLen=COINSPARK_MIN(metadataEnd-metadataPtr, COINSPARK_GENESIS_HASH_MAX_LEN); // apply maximum
    memcpy(genesis->assetHash, metadataPtr, genesis->assetHashLen);
    metadataPtr+=genesis->assetHashLen;
    
//  Return validity
    
    return CoinSparkGenesisIsValid(genesis);
    
    cannotDecodeGenesis:
    return FALSE;
}

CoinSparkSatoshiQty CoinSparkGenesisCalcMinFee(const CoinSparkGenesis *genesis, const CoinSparkSatoshiQty* outputsSatoshis,
                                               const bool* outputsRegular, const int countOutputs)
{
    return CountNonLastRegularOutputs(outputsRegular, countOutputs)*GetMinFeeBasis(outputsSatoshis, outputsRegular, countOutputs);
}

void CoinSparkAssetRefClear(CoinSparkAssetRef *assetRef)
{
    assetRef->blockNum=0;
    assetRef->txOffset=0;
    memset(assetRef->txIDPrefix, 0, COINSPARK_ASSETREF_TXID_PREFIX_LEN);
}

static bool CoinSparkAssetRefToStringInner(const CoinSparkAssetRef *assetRef, char* string, const size_t stringMaxLen, bool headers)
{
    char buffer[1024], hex[17], *bufferPtr;
    size_t bufferLength, copyLength;
    
    bufferPtr=buffer;
    
    if (headers)
        bufferPtr+=sprintf(bufferPtr, "COINSPARK ASSET REFERENCE\n");
    
    bufferPtr+=sprintf(bufferPtr, "Genesis block index: %lld (small endian hex %s)\n", (long long)assetRef->blockNum, UnsignedToSmallEndianHex(assetRef->blockNum, 4, hex));
    bufferPtr+=sprintf(bufferPtr, " Genesis txn offset: %lld (small endian hex %s)\n", (long long)assetRef->txOffset, UnsignedToSmallEndianHex(assetRef->txOffset, 4, hex));
    bufferPtr+=sprintf(bufferPtr, "Genesis txid prefix: %s\n", BinaryToHex(assetRef->txIDPrefix, sizeof(assetRef->txIDPrefix), hex));
    
    if (headers)
        bufferPtr+=sprintf(bufferPtr, "END COINSPARK ASSET REFERENCE\n\n");

    bufferLength=bufferPtr-buffer;
    copyLength=COINSPARK_MIN(bufferLength, stringMaxLen-1);
    memcpy(string, buffer, copyLength);
    string[copyLength]=0x00;
    
    return (copyLength==bufferLength);
}

bool CoinSparkAssetRefToString(const CoinSparkAssetRef *assetRef, char* string, const size_t stringMaxLen)
{
    return CoinSparkAssetRefToStringInner(assetRef, string, stringMaxLen, TRUE);
}

bool CoinSparkAssetRefIsValid(const CoinSparkAssetRef *assetRef)
{
    if (assetRef->blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) {
        if ( (assetRef->blockNum<0) || (assetRef->blockNum>COINSPARK_ASSETREF_BLOCK_NUM_MAX) )
            goto assetRefIsInvalid;
        
        if ( (assetRef->txOffset<0) || (assetRef->txOffset>COINSPARK_ASSETREF_TX_OFFSET_MAX) )
            goto assetRefIsInvalid;
    }

    return TRUE;

    assetRefIsInvalid:
    return FALSE;
}

bool CoinSparkAssetRefMatch(const CoinSparkAssetRef* assetRef1, const CoinSparkAssetRef* assetRef2)
{
    return (!memcmp(assetRef1->txIDPrefix, assetRef2->txIDPrefix, COINSPARK_ASSETREF_TXID_PREFIX_LEN)) &&
    (assetRef1->txOffset==assetRef2->txOffset) &&
    (assetRef1->blockNum==assetRef2->blockNum);
}

size_t CoinSparkAssetRefEncode(const CoinSparkAssetRef *assetRef, char* string, const size_t stringMaxLen)
{
    char buffer[1024];
    size_t bufferLength, copyLength;
    int txIDPrefixInteger;
        
    if (!CoinSparkAssetRefIsValid(assetRef))
        goto cannotEncodeAssetRef;

    txIDPrefixInteger=256*((unsigned char*)assetRef->txIDPrefix)[1]+((unsigned char*)assetRef->txIDPrefix)[0];
    
    bufferLength=sprintf(buffer, "%lld-%lld-%d", (long long)assetRef->blockNum, (long long)assetRef->txOffset, txIDPrefixInteger);
        
    copyLength=COINSPARK_MIN(bufferLength, stringMaxLen-1);
    memcpy(string, buffer, copyLength);
    string[copyLength]=0x00;
    
    return copyLength;
    
    cannotEncodeAssetRef:
    return 0;
}

bool CoinSparkAssetRefDecode(CoinSparkAssetRef *assetRef, const char* string, const size_t stringLen)
{
    char buffer[1024];
    int txIDPrefixInteger;
    long long blockNum, txOffset;
    
    if (stringLen>=sizeof(buffer))
        return FALSE;
    
    memcpy(buffer, string, stringLen);
    buffer[stringLen]=0; // copy to our buffer and null terminate to allow scanf
    
    if (strchr(buffer, '+')) // special check for '+' character which would be accepted by sscanf() below
        return FALSE;
           
    if (sscanf(buffer, "%lld-%lld-%d", &blockNum, &txOffset, &txIDPrefixInteger)!=3)
        return FALSE;
    
    if ( (txIDPrefixInteger<0) || (txIDPrefixInteger>0xFFFF) )
        return FALSE;
    
    assetRef->blockNum=blockNum;
    assetRef->txOffset=txOffset;
    ((unsigned char*)assetRef->txIDPrefix)[0]=txIDPrefixInteger%256;
    ((unsigned char*)assetRef->txIDPrefix)[1]=txIDPrefixInteger/256;
    
    return CoinSparkAssetRefIsValid(assetRef);
}

void CoinSparkTransferClear(CoinSparkTransfer* transfer)
{
    CoinSparkAssetRefClear(&transfer->assetRef);
    transfer->inputs.first=0;
    transfer->inputs.count=0;
    transfer->outputs.first=0;
    transfer->outputs.count=0;
    transfer->qtyPerOutput=0;
}

static bool CoinSparkTransferToStringInner(const CoinSparkTransfer* transfer, char* string, const size_t stringMaxLen, bool headers)
{
    char buffer[1024], assetRefString[256], hex1[17], hex2[17], *bufferPtr;
    size_t bufferLength, copyLength;
    int qtyMantissa, qtyExponent;
    CoinSparkAssetQty encodeQuantity;
    bool isDefaultRoute;
    
    bufferPtr=buffer;

    if (headers)
        bufferPtr+=sprintf(bufferPtr, "COINSPARK TRANSFER\n");

    isDefaultRoute=(transfer->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);
    
    if (isDefaultRoute)
        bufferPtr+=sprintf(bufferPtr, "      Default route:\n");
    
    else {
        CoinSparkAssetRefToStringInner(&transfer->assetRef, bufferPtr, buffer+stringMaxLen-bufferPtr, FALSE);
        bufferPtr+=strlen(bufferPtr);
        
        CoinSparkAssetRefEncode(&transfer->assetRef, assetRefString, sizeof(assetRefString));
        bufferPtr+=sprintf(bufferPtr, "    Asset reference: %s\n", assetRefString);
    }
    
    if (transfer->inputs.count>0) {
        if (transfer->inputs.count>1)
            bufferPtr+=sprintf(bufferPtr, "             Inputs: %d - %d (count %d)", transfer->inputs.first,
                               transfer->inputs.first+transfer->inputs.count-1, transfer->inputs.count);
        else
            bufferPtr+=sprintf(bufferPtr, "              Input: %d", transfer->inputs.first);
    } else
        bufferPtr+=sprintf(bufferPtr, "             Inputs: none");
    
    bufferPtr+=sprintf(bufferPtr, " (small endian hex: first %s count %s)\n", UnsignedToSmallEndianHex(transfer->inputs.first, 2, hex1), UnsignedToSmallEndianHex(transfer->inputs.count, 2, hex2));
    
    if (transfer->outputs.count>0) {
        if ((transfer->outputs.count>1) && !isDefaultRoute)
            bufferPtr+=sprintf(bufferPtr, "            Outputs: %d - %d (count %d)", transfer->outputs.first,
                               transfer->outputs.first+transfer->outputs.count-1, transfer->outputs.count);
        else
            bufferPtr+=sprintf(bufferPtr, "             Output: %d", transfer->outputs.first);
    } else
        bufferPtr+=sprintf(bufferPtr, "            Outputs: none");
    
    bufferPtr+=sprintf(bufferPtr, " (small endian hex: first %s count %s)\n", UnsignedToSmallEndianHex(transfer->outputs.first, 2, hex1), UnsignedToSmallEndianHex(transfer->outputs.count, 2, hex2));
    
    if (!isDefaultRoute) {
        bufferPtr+=sprintf(bufferPtr, "     Qty per output: %lld (small endian hex %s", (long long)transfer->qtyPerOutput,     UnsignedToSmallEndianHex(transfer->qtyPerOutput, 8, hex1));
    
        if (QtyToMantissaExponent(transfer->qtyPerOutput, 0, COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX, COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX, &qtyMantissa, &qtyExponent)==transfer->qtyPerOutput) {
            encodeQuantity=(qtyExponent*COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+qtyMantissa)&COINSPARK_TRANSFER_QTY_FLOAT_MASK;
            
            bufferPtr+=sprintf(bufferPtr, ", as float %s", UnsignedToSmallEndianHex(encodeQuantity, COINSPARK_TRANSFER_QTY_FLOAT_LENGTH, hex1));
        }
        
        bufferPtr+=sprintf(bufferPtr, ")\n");
    }
    
    if (headers)
        bufferPtr+=sprintf(bufferPtr, "END COINSPARK TRANSFER\n\n");

    bufferLength=bufferPtr-buffer;
    copyLength=COINSPARK_MIN(bufferLength, stringMaxLen-1);
    memcpy(string, buffer, copyLength);
    string[copyLength]=0x00;
    
    return (copyLength==bufferLength);
}

bool CoinSparkTransferToString(const CoinSparkTransfer* transfer, char* string, const size_t stringMaxLen)
{
    return CoinSparkTransferToStringInner(transfer, string, stringMaxLen, TRUE);
}

bool CoinSparkTransfersToString(const CoinSparkTransfer* transfers, const int countTransfers, char* string, const size_t stringMaxLen)
{
    char *buffer, *bufferPtr;
    int transferIndex;
    size_t bufferLength, copyLength, bufferCapacity;
    int ordering[1024];
    
    bufferCapacity=1024*(1+countTransfers);
    buffer=(char*)malloc(bufferCapacity);
    bufferPtr=buffer;
    
    if (countTransfers>(sizeof(ordering)/sizeof(*ordering)))
        return FALSE;
    
    TransfersGroupOrdering(transfers, ordering, countTransfers);
    
    bufferPtr+=sprintf(bufferPtr, "COINSPARK TRANSFERS\n");
    
    for (transferIndex=0; transferIndex<countTransfers; transferIndex++) {
        if (transferIndex>0)
            bufferPtr+=sprintf(bufferPtr, "\n");
        
        CoinSparkTransferToStringInner(transfers+ordering[transferIndex], bufferPtr, buffer+bufferCapacity-bufferPtr, FALSE);
        bufferPtr+=strlen(bufferPtr);
    }
    
    bufferPtr+=sprintf(bufferPtr, "END COINSPARK TRANSFERS\n\n");
    
    bufferLength=bufferPtr-buffer;
    copyLength=COINSPARK_MIN(bufferLength, stringMaxLen-1);
    memcpy(string, buffer, copyLength);
    string[copyLength]=0x00;
    free(buffer);
    
    return (copyLength==bufferLength);
}

bool CoinSparkTransferMatch(const CoinSparkTransfer* transfer1, const CoinSparkTransfer* transfer2)
{
    bool partialMatch;
    
    partialMatch=(transfer1->inputs.first==transfer2->inputs.first) && (transfer1->inputs.count==transfer2->inputs.count) &&
    (transfer1->outputs.first==transfer2->outputs.first);
    
    if (transfer1->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
        return (transfer2->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) && partialMatch;
        
    else
        return CoinSparkAssetRefMatch(&transfer1->assetRef, &transfer2->assetRef) && partialMatch &&
            (transfer1->outputs.count==transfer2->outputs.count) && (transfer1->qtyPerOutput==transfer2->qtyPerOutput);
}

bool CoinSparkTransfersMatch(const CoinSparkTransfer* transfers1, const CoinSparkTransfer* transfers2, int countTransfers, bool strict)
{
    int ordering1[1024], ordering2[1024];
    int transferIndex;
    
    if (strict) {
        for (transferIndex=0; transferIndex<countTransfers; transferIndex++)
            if (!CoinSparkTransferMatch(transfers1+transferIndex, transfers2+transferIndex))
                return FALSE;
        
    } else {
        if (countTransfers>(sizeof(ordering1)/sizeof(*ordering1)))
            return FALSE;
        
        TransfersGroupOrdering(transfers1, ordering1, countTransfers);
        TransfersGroupOrdering(transfers2, ordering2, countTransfers);
        
        for (transferIndex=0; transferIndex<countTransfers; transferIndex++)
            if (!CoinSparkTransferMatch(transfers1+ordering1[transferIndex], transfers2+ordering2[transferIndex]))
                return FALSE;
    }
    
    return TRUE;
}

bool CoinSparkTransferIsValid(const CoinSparkTransfer* transfer)
{
    if (!CoinSparkAssetRefIsValid(&transfer->assetRef))
        goto transferIsInvalid;
    
    if ( (transfer->inputs.first<0) || (transfer->inputs.first>COINSPARK_IO_INDEX_MAX) )
        goto transferIsInvalid;
    
    if ( (transfer->inputs.count<0) || (transfer->inputs.count>COINSPARK_IO_INDEX_MAX) )
        goto transferIsInvalid;
    
    if ( (transfer->outputs.first<0) || (transfer->outputs.first>COINSPARK_IO_INDEX_MAX) )
        goto transferIsInvalid;
    
    if ( (transfer->outputs.count<0) || (transfer->outputs.count>COINSPARK_IO_INDEX_MAX) )
        goto transferIsInvalid;
    
    if ( (transfer->qtyPerOutput<0) || (transfer->qtyPerOutput>COINSPARK_ASSET_QTY_MAX) )
        goto transferIsInvalid;
    
    return TRUE;
    
    transferIsInvalid:
    return FALSE;
}

bool CoinSparkTransfersAreValid(const CoinSparkTransfer* transfers, const int countTransfers)
{
    int transferIndex;
    
    for (transferIndex=0; transferIndex<countTransfers; transferIndex++)
        if (!CoinSparkTransferIsValid(transfers+transferIndex))
            return FALSE;
            
    return TRUE;
}

static size_t CoinSparkTransferEncode(const CoinSparkTransfer* transfer, const CoinSparkTransfer* previousTransfer, char* metadata, const size_t metadataMaxLen, const int countInputs, const int countOutputs)
{
    char *metadataPtr, *metadataEnd;
    PackingOptions inputPackingOptions, outputPackingOptions;
    char packing, packingExtendInput, packingExtendOutput, packingExtend;
    PackingByteCounts counts;
    CoinSparkAssetQty encodeQuantity;
    int qtyMantissa, qtyExponent;
    bool isDefaultRoute;
    
    if (!CoinSparkTransferIsValid(transfer))
        goto cannotEncodeTransfer;
    
    metadataPtr=metadata;
    metadataEnd=metadataPtr+metadataMaxLen;
    packing=0;
    packingExtend=0;
    isDefaultRoute=(transfer->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);
    
//  Packing for genesis reference
    
    if (isDefaultRoute) {
        if (previousTransfer && (previousTransfer->assetRef.blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE))
            goto cannotEncodeTransfer; // default route transfers have to come at the start
        
        packing|=COINSPARK_PACKING_GENESIS_PREV;
    
    } else {
        
        if (previousTransfer && CoinSparkAssetRefMatch(&transfer->assetRef, &previousTransfer->assetRef))
            packing|=COINSPARK_PACKING_GENESIS_PREV;
     
        else if (transfer->assetRef.blockNum <= COINSPARK_UNSIGNED_3_BYTES_MAX) {
            if (transfer->assetRef.txOffset <= COINSPARK_UNSIGNED_3_BYTES_MAX)
                packing|=COINSPARK_PACKING_GENESIS_3_3_BYTES;
            else if (transfer->assetRef.txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX)
                packing|=COINSPARK_PACKING_GENESIS_3_4_BYTES;
            else
                goto cannotEncodeTransfer;
        
        } else if ( (transfer->assetRef.blockNum <= COINSPARK_UNSIGNED_4_BYTES_MAX) && (transfer->assetRef.txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX) )
            packing|=COINSPARK_PACKING_GENESIS_4_4_BYTES;
        
        else
            goto cannotEncodeTransfer;
    }
    
//  Packing for input and output indices
    
    GetPackingOptions(previousTransfer ? &previousTransfer->inputs : NULL, &transfer->inputs, countInputs, inputPackingOptions);
    GetPackingOptions(previousTransfer ? &previousTransfer->outputs : NULL, &transfer->outputs, countOutputs, outputPackingOptions);
    
    if (inputPackingOptions[_0P] && outputPackingOptions[_0P])
        packing|=COINSPARK_PACKING_INDICES_0P_0P;
    else if (inputPackingOptions[_0P] && outputPackingOptions[_1S])
        packing|=COINSPARK_PACKING_INDICES_0P_1S;
    else if (inputPackingOptions[_0P] && outputPackingOptions[_ALL])
        packing|=COINSPARK_PACKING_INDICES_0P_ALL;
    else if (inputPackingOptions[_1S] && outputPackingOptions[_0P])
        packing|=COINSPARK_PACKING_INDICES_1S_0P;
    else if (inputPackingOptions[_ALL] && outputPackingOptions[_0P])
        packing|=COINSPARK_PACKING_INDICES_ALL_0P;
    else if (inputPackingOptions[_ALL] && outputPackingOptions[_1S])
        packing|=COINSPARK_PACKING_INDICES_ALL_1S;
    else if (inputPackingOptions[_ALL] && outputPackingOptions[_ALL])
        packing|=COINSPARK_PACKING_INDICES_ALL_ALL;
    
    else { // we need the second (extended) packing byte
        packing|=COINSPARK_PACKING_INDICES_EXTEND;
        
        if (!EncodePackingExtend(inputPackingOptions, &packingExtendInput))
            goto cannotEncodeTransfer;
        
        if (!EncodePackingExtend(outputPackingOptions, &packingExtendOutput))
            goto cannotEncodeTransfer;
        
        packingExtend=(packingExtendInput<<COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) | (packingExtendOutput<<COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT);
    }
    
//  Packing for quantity
    
    encodeQuantity=transfer->qtyPerOutput;
    
    if (transfer->qtyPerOutput==(previousTransfer ? previousTransfer->qtyPerOutput : 1))
        packing|=COINSPARK_PACKING_QUANTITY_1P;
    else if (transfer->qtyPerOutput>=COINSPARK_ASSET_QTY_MAX)
        packing|=COINSPARK_PACKING_QUANTITY_MAX;
    else if (transfer->qtyPerOutput<=COINSPARK_UNSIGNED_BYTE_MAX)
        packing|=COINSPARK_PACKING_QUANTITY_1_BYTE;
    else if (transfer->qtyPerOutput<=COINSPARK_UNSIGNED_2_BYTES_MAX)
        packing|=COINSPARK_PACKING_QUANTITY_2_BYTES;
    else if (QtyToMantissaExponent(transfer->qtyPerOutput, 0, COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX,
            COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX, &qtyMantissa, &qtyExponent)==transfer->qtyPerOutput) {
        packing|=COINSPARK_PACKING_QUANTITY_FLOAT;
        encodeQuantity=(qtyExponent*COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+qtyMantissa)&COINSPARK_TRANSFER_QTY_FLOAT_MASK;
    } else if (transfer->qtyPerOutput<=COINSPARK_UNSIGNED_3_BYTES_MAX)
        packing|=COINSPARK_PACKING_QUANTITY_3_BYTES;
    else if (transfer->qtyPerOutput<=COINSPARK_UNSIGNED_4_BYTES_MAX)
        packing|=COINSPARK_PACKING_QUANTITY_4_BYTES;
    else
        packing|=COINSPARK_PACKING_QUANTITY_6_BYTES;

//  Write out the actual data
    
    #define WRITE_BYTE_FIELD(bytes, source) \
        if (bytes>0) { \
            if ( (metadataPtr+(bytes)) <= metadataEnd ) { \
                memcpy(metadataPtr, &source, bytes); \
                metadataPtr+=(bytes); \
            } else \
                goto cannotEncodeTransfer; \
        }
    
    #define WRITE_UNSIGNED_FIELD(bytes, source) \
        if (bytes>0) { \
            if ( (metadataPtr+(bytes)) <= metadataEnd ) { \
                if (!WriteSmallEndianUnsigned(source, metadataPtr, bytes)) \
                    goto cannotEncodeTransfer; \
                metadataPtr+=(bytes); \
            } else \
                goto cannotEncodeTransfer; \
        }
    
    PackingToByteCounts(packing, packingExtend, &counts);
    
    WRITE_BYTE_FIELD(1, packing);
   
    if ((packing & COINSPARK_PACKING_INDICES_MASK)==COINSPARK_PACKING_INDICES_EXTEND)
        WRITE_BYTE_FIELD(1, packingExtend);
    
    WRITE_UNSIGNED_FIELD(counts.blockNumBytes, transfer->assetRef.blockNum);
    WRITE_UNSIGNED_FIELD(counts.txOffsetBytes, transfer->assetRef.txOffset);
    WRITE_BYTE_FIELD(counts.txIDPrefixBytes, transfer->assetRef.txIDPrefix);
    WRITE_UNSIGNED_FIELD(counts.firstInputBytes, transfer->inputs.first);
    WRITE_UNSIGNED_FIELD(counts.countInputsBytes, transfer->inputs.count);
    WRITE_UNSIGNED_FIELD(counts.firstOutputBytes, transfer->outputs.first);
    WRITE_UNSIGNED_FIELD(counts.countOutputsBytes, transfer->outputs.count);
    WRITE_UNSIGNED_FIELD(counts.quantityBytes, encodeQuantity);
    
//  Clear up and return
    
    return metadataPtr-metadata;
    
    cannotEncodeTransfer:
    return 0;
}

size_t CoinSparkTransfersEncode(const CoinSparkTransfer* transfers, const int countTransfers, const int countInputs, const int countOutputs, char* metadata, const size_t metadataMaxLen)
{
    char *metadataPtr, *metadataEnd;
    int transferIndex;
    size_t oneBytesUsed;
    const CoinSparkTransfer *previousTransfer;
    int ordering[1024];
    
    metadataPtr=metadata;
    metadataEnd=metadataPtr+metadataMaxLen;
    
    if (countTransfers>(sizeof(ordering)/sizeof(*ordering)))
        return 0; // too many for statically sized array
    
//  4-character identifier
    
    if ((metadataPtr+COINSPARK_METADATA_IDENTIFIER_LEN+1)<=metadataEnd) {
        memcpy(metadataPtr, COINSPARK_METADATA_IDENTIFIER, COINSPARK_METADATA_IDENTIFIER_LEN);
        metadataPtr+=COINSPARK_METADATA_IDENTIFIER_LEN;
        *metadataPtr++=COINSPARK_TRANSFERS_PREFIX;
    } else
        return 0; // return straight away if no space for header

//  Encode each transfer, grouping by asset reference, but preserving original order otherwise
    
    TransfersGroupOrdering(transfers, ordering, countTransfers);
    
    previousTransfer=NULL;
    
    for (transferIndex=0; transferIndex<countTransfers; transferIndex++) {
        oneBytesUsed=CoinSparkTransferEncode(&transfers[ordering[transferIndex]], previousTransfer,
            metadataPtr, metadataEnd-metadataPtr, countInputs, countOutputs);
        
        previousTransfer=&transfers[ordering[transferIndex]];
        
        if (oneBytesUsed>0)
            metadataPtr+=oneBytesUsed;
        else
            return 0;
    }

//  Return number of bytes used
    
    return metadataPtr-metadata;
}

static size_t CoinSparkTransferDecode(const char* metadata, const size_t metadataLen, const CoinSparkTransfer* previousTransfer, CoinSparkTransfer* transfer, const int countInputs, const int countOutputs)
{
    const char *metadataPtr, *metadataEnd;
    char packing, packingExtend;
    PackingType inputPackingType, outputPackingType;
    PackingByteCounts counts;
    CoinSparkAssetQty decodeQuantity;

    metadataPtr=metadata;
    metadataEnd=metadataPtr+metadataLen;
    
//  Extract packing
    
    if (metadataPtr<metadataEnd)
        packing=*metadataPtr++;
    else
        goto cannotDecodeTransfer;

//  Packing for genesis reference
    
    switch (packing & COINSPARK_PACKING_GENESIS_MASK)
    {
        case COINSPARK_PACKING_GENESIS_PREV:
            if (previousTransfer)
                transfer->assetRef=previousTransfer->assetRef;

            else { // it's for a default route
                transfer->assetRef.blockNum=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE;
                transfer->assetRef.txOffset=0;
                memset(&transfer->assetRef.txIDPrefix, 0, sizeof(transfer->assetRef.txIDPrefix));
            }
            break;
    }
    
//  Packing for input and output indices
    
    if ((packing & COINSPARK_PACKING_INDICES_MASK) == COINSPARK_PACKING_INDICES_EXTEND) { // we're using second packing metadata byte
        if (metadataPtr<metadataEnd)
            packingExtend=*metadataPtr++;
        else
            goto cannotDecodeTransfer;
    
        if (!DecodePackingExtend((packingExtend>>COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK, &inputPackingType))
            goto cannotDecodeTransfer;
        
        if (!DecodePackingExtend((packingExtend>>COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK, &outputPackingType))
            goto cannotDecodeTransfer;
        
    } else { // not using second packing metadata byte
        packingExtend=0;
        
        switch (packing & COINSPARK_PACKING_INDICES_MASK) // input packing
        {
            case COINSPARK_PACKING_INDICES_0P_0P:
            case COINSPARK_PACKING_INDICES_0P_1S:
            case COINSPARK_PACKING_INDICES_0P_ALL:
                inputPackingType=_0P;
                break;
                
            case COINSPARK_PACKING_INDICES_1S_0P:
                inputPackingType=_1S;
                break;
            
            case COINSPARK_PACKING_INDICES_ALL_0P:
            case COINSPARK_PACKING_INDICES_ALL_1S:
            case COINSPARK_PACKING_INDICES_ALL_ALL:
                inputPackingType=_ALL;
                break;
        }
        
        switch (packing & COINSPARK_PACKING_INDICES_MASK) // output packing
        {
            case COINSPARK_PACKING_INDICES_0P_0P:
            case COINSPARK_PACKING_INDICES_1S_0P:
            case COINSPARK_PACKING_INDICES_ALL_0P:
                outputPackingType=_0P;
                break;
                
            case COINSPARK_PACKING_INDICES_0P_1S:
            case COINSPARK_PACKING_INDICES_ALL_1S:
                outputPackingType=_1S;
                break;
                
            case COINSPARK_PACKING_INDICES_0P_ALL:
            case COINSPARK_PACKING_INDICES_ALL_ALL:
                outputPackingType=_ALL;
                break;
        }
    }
    
//  Final stage of packing for input and output indices
    
    PackingTypeToValues(inputPackingType, previousTransfer ? &previousTransfer->inputs : NULL, countInputs, &transfer->inputs);
    PackingTypeToValues(outputPackingType, previousTransfer ? &previousTransfer->outputs : NULL, countOutputs, &transfer->outputs);
    
//  Read in the fields as appropriate
    
    PackingToByteCounts(packing, packingExtend, &counts);
    
    #define READ_BYTE_FIELD(bytes, destination) \
        if (bytes>0) { \
            if ( (metadataPtr+(bytes)) <= metadataEnd ) { \
                memcpy(&destination, metadataPtr, bytes); \
                metadataPtr+=(bytes); \
            } else \
                goto cannotDecodeTransfer; \
        }
    
    #define READ_UNSIGNED_FIELD(bytes, destination, cast) \
        if (bytes>0) { \
            if ( (metadataPtr+(bytes)) <= metadataEnd ) { \
                (destination)=(cast)ReadSmallEndianUnsigned(metadataPtr, bytes); \
                metadataPtr+=(bytes); \
            } else \
                goto cannotDecodeTransfer; \
        }
    
    READ_UNSIGNED_FIELD(counts.blockNumBytes, transfer->assetRef.blockNum, int64_t);
    READ_UNSIGNED_FIELD(counts.txOffsetBytes, transfer->assetRef.txOffset, int64_t);
    READ_BYTE_FIELD(counts.txIDPrefixBytes, transfer->assetRef.txIDPrefix);
    READ_UNSIGNED_FIELD(counts.firstInputBytes, transfer->inputs.first, CoinSparkIOIndex);
    READ_UNSIGNED_FIELD(counts.countInputsBytes, transfer->inputs.count, CoinSparkIOIndex);
    READ_UNSIGNED_FIELD(counts.firstOutputBytes, transfer->outputs.first, CoinSparkIOIndex);
    READ_UNSIGNED_FIELD(counts.countOutputsBytes, transfer->outputs.count, CoinSparkIOIndex);
    READ_UNSIGNED_FIELD(counts.quantityBytes, decodeQuantity, CoinSparkAssetQty);
    
//  Finish up reading in quantity
    
    switch (packing & COINSPARK_PACKING_QUANTITY_MASK)
    {
        case COINSPARK_PACKING_QUANTITY_1P:
            if (previousTransfer)
                transfer->qtyPerOutput=previousTransfer->qtyPerOutput;
            else
                transfer->qtyPerOutput=1;
            break;
            
        case COINSPARK_PACKING_QUANTITY_MAX:
            transfer->qtyPerOutput=COINSPARK_ASSET_QTY_MAX;
            break;
            
        case COINSPARK_PACKING_QUANTITY_FLOAT:
            decodeQuantity&=COINSPARK_TRANSFER_QTY_FLOAT_MASK;
            transfer->qtyPerOutput=MantissaExponentToQty(decodeQuantity%COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE,
                (int)(decodeQuantity/COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE));
            break;
        
        default:
            transfer->qtyPerOutput=decodeQuantity;
            break;
    }
    
//  Finish up and return
    
    if (!CoinSparkTransferIsValid(transfer))
        goto cannotDecodeTransfer;
    
    return metadataPtr-metadata;

    cannotDecodeTransfer:
    return 0;
}

int CoinSparkTransfersDecodeCount(const char* metadata, const size_t metadataLen)
{
    return CoinSparkTransfersDecode(NULL, 0, COINSPARK_IO_INDEX_MAX, COINSPARK_IO_INDEX_MAX, metadata, metadataLen);
}

int CoinSparkTransfersDecode(CoinSparkTransfer* transfers, const int maxTransfers, const int countInputs, const int countOutputs,
                             const char* metadata, const size_t metadataLen)
{
    const char *metadataPtr, *metadataEnd;
    CoinSparkTransfer transfer, previousTransfer;
    int countTransfers;
    size_t transferBytesUsed;
    
    metadataPtr=metadata;
    metadataEnd=metadataPtr+metadataLen;

    if (!LocateMetadataRange(&metadataPtr, &metadataEnd, COINSPARK_TRANSFERS_PREFIX))
        return 0;

//  Iterate over list
    
    countTransfers=0;
    while (metadataPtr<metadataEnd) {
        transferBytesUsed=CoinSparkTransferDecode(metadataPtr, metadataEnd-metadataPtr, countTransfers ? &previousTransfer : NULL,
                                                  &transfer, countInputs, countOutputs);
        
        if (transferBytesUsed>0) {
            if (countTransfers<maxTransfers)
                transfers[countTransfers]=transfer; // copy across if still space
            
            countTransfers++;
            metadataPtr+=transferBytesUsed;
            previousTransfer=transfer;

        } else
            return 0; // something was invalid
    }
    
//  Return count
    
    return countTransfers;
}

CoinSparkSatoshiQty CoinSparkTransfersCalcMinFee(const CoinSparkTransfer* transfers, const int countTransfers,
                                                 const int countInputs, const int countOutputs,
                                                 const CoinSparkSatoshiQty* outputsSatoshis, const bool* outputsRegular)
{
    int transfersToCover, transferIndex, outputIndex, lastOutputIndex;
    
    transfersToCover=0;
    
    for (transferIndex=0; transferIndex<countTransfers; transferIndex++) {
        if (
            (transfers[transferIndex].assetRef.blockNum != COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) && // don't count default routes
            (transfers[transferIndex].inputs.count>0) &&
            (transfers[transferIndex].inputs.first<countInputs) // only count if at least one valid input index
        ) {
            outputIndex=COINSPARK_MAX(transfers[transferIndex].outputs.first, 0);
            lastOutputIndex=COINSPARK_MIN(transfers[transferIndex].outputs.first+transfers[transferIndex].outputs.count, countOutputs)-1;
            
            for (; outputIndex<=lastOutputIndex; outputIndex++)
                if (outputsRegular[outputIndex])
                    transfersToCover++;
        }
    }
    
    return transfersToCover*GetMinFeeBasis(outputsSatoshis, outputsRegular, countOutputs);
}

void CoinSparkGenesisApply(const CoinSparkGenesis* genesis, const bool* outputsRegular,
                           CoinSparkAssetQty* outputBalances, const int countOutputs)
{
    int divideOutputs, outputIndex, lastRegularOutput;
    CoinSparkAssetQty genesisQty, qtyPerOutput, extraFirstOutput;
    
    lastRegularOutput=GetLastRegularOutput(outputsRegular, countOutputs);
    divideOutputs=CountNonLastRegularOutputs(outputsRegular, countOutputs);
    genesisQty=CoinSparkGenesisGetQty(genesis);
    
    if (divideOutputs==0)
        qtyPerOutput=0;
    else
        qtyPerOutput=genesisQty/divideOutputs; // rounds down
    
    extraFirstOutput=genesisQty-qtyPerOutput*divideOutputs;

    for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
        if (outputsRegular[outputIndex] && (outputIndex!=lastRegularOutput)) {
            outputBalances[outputIndex]=qtyPerOutput+extraFirstOutput;
            extraFirstOutput=0; // so it will only contribute to the first
        } else
            outputBalances[outputIndex]=0;
}

void CoinSparkTransfersApply(const CoinSparkAssetRef* assetRef, const CoinSparkGenesis* genesis,
                             const CoinSparkTransfer* transfers, const int countTransfers,
                             const CoinSparkAssetQty* inputBalances, const int countInputs,
                             const bool* outputsRegular, CoinSparkAssetQty* outputBalances, const int countOutputs)
{
    int transferIndex, outputIndex, inputIndex, lastInputIndex, lastOutputIndex;
    int *inputDefaultOutput;
    CoinSparkAssetQty *inputsRemaining, transferRemaining, transferQuantity;

//  Copy all input quantities and zero output quantities
    
    inputsRemaining=malloc(countInputs*sizeof(*inputsRemaining));
    for (inputIndex=0; inputIndex<countInputs; inputIndex++)
        inputsRemaining[inputIndex]=inputBalances[inputIndex];
    
    for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
        outputBalances[outputIndex]=0;

//  Perform explicit transfers (i.e. not default routes)
    
    for (transferIndex=0; transferIndex<countTransfers; transferIndex++) {
        if (CoinSparkAssetRefMatch(assetRef, &transfers[transferIndex].assetRef)) { // will exclude default route entries
            inputIndex=COINSPARK_MAX(transfers[transferIndex].inputs.first, 0);
            outputIndex=COINSPARK_MAX(transfers[transferIndex].outputs.first, 0);
            
            lastInputIndex=COINSPARK_MIN(inputIndex+transfers[transferIndex].inputs.count, countInputs)-1;
            lastOutputIndex=COINSPARK_MIN(outputIndex+transfers[transferIndex].outputs.count, countOutputs)-1;
            
            for (; outputIndex<=lastOutputIndex; outputIndex++)
                if (outputsRegular[outputIndex]) {
                    transferRemaining=transfers[transferIndex].qtyPerOutput;
                    
                    while (inputIndex<=lastInputIndex) {
                        transferQuantity=COINSPARK_MIN(transferRemaining, inputsRemaining[inputIndex]);
                        
                        if (transferQuantity>0) { // skip all this if nothing is to be transferred (branch not really necessary)
                            inputsRemaining[inputIndex]-=transferQuantity;
                            transferRemaining-=transferQuantity;
                            outputBalances[outputIndex]+=transferQuantity;
                        }
                        
                        if (transferRemaining>0)
                            inputIndex++; // move to next input since this one is drained
                        else
                            break; // stop if we have nothing left to transfer
                    }
                }
        }
    }

//  Apply payment charges to all quantities not routed by default
    
    for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
        if (outputsRegular[outputIndex])
            outputBalances[outputIndex]=CoinSparkGenesisCalcNet(genesis, outputBalances[outputIndex]);
    
//  Send remaining quantities to default outputs
    
    inputDefaultOutput=(int*)malloc(sizeof(*inputDefaultOutput)*countInputs);
    GetDefaultRouteMap(transfers, countTransfers, countInputs, countOutputs, outputsRegular, inputDefaultOutput);
    
    for (inputIndex=0; inputIndex<countInputs; inputIndex++) {
        outputIndex=inputDefaultOutput[inputIndex];
        
        if (outputIndex<countOutputs) // could be out of range if there are no regular outputs
            outputBalances[outputIndex]+=inputsRemaining[inputIndex];
    }
    
    free(inputDefaultOutput);
    free(inputsRemaining);
}

void CoinSparkTransfersApplyNone(const CoinSparkAssetRef* assetRef, const CoinSparkGenesis* genesis,
                              const CoinSparkAssetQty* inputBalances, const int countInputs,
                              const bool* outputsRegular, CoinSparkAssetQty* outputBalances, const int countOutputs)
{
    CoinSparkTransfersApply(assetRef, genesis, NULL, 0, inputBalances, countInputs, outputsRegular, outputBalances, countOutputs);
}

void CoinSparkTransfersDefaultOutputs(const CoinSparkTransfer* transfers, const int countTransfers, const int countInputs,
                                const bool* outputsRegular, bool* outputsDefault, const int countOutputs)
{
    int *inputDefaultOutput, outputIndex, inputIndex;

    for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
        outputsDefault[outputIndex]=FALSE;
    
    inputDefaultOutput=(int*)malloc(sizeof(*inputDefaultOutput)*countInputs);
    GetDefaultRouteMap(transfers, countTransfers, countInputs, countOutputs, outputsRegular, inputDefaultOutput);
    
    for (inputIndex=0; inputIndex<countInputs; inputIndex++) {
        outputIndex=inputDefaultOutput[inputIndex];
        
        if (outputIndex<countOutputs)
            outputsDefault[outputIndex]=TRUE;
    }
    
    free(inputDefaultOutput);
}

bool CoinSparkPaymentRefToString(const CoinSparkPaymentRef paymentRef, char* string, const size_t stringMaxLen)
{
    char buffer[1024], hex[17], *bufferPtr;
    size_t bufferLength, copyLength;
    
    bufferPtr=buffer;

    bufferPtr+=sprintf(bufferPtr, "COINSPARK PAYMENT REFERENCE\n");
    bufferPtr+=sprintf(bufferPtr, "%lld (small endian hex %s)\n", (long long)paymentRef, UnsignedToSmallEndianHex(paymentRef, 8, hex));
    bufferPtr+=sprintf(bufferPtr, "END COINSPARK PAYMENT REFERENCE\n\n");
    
    bufferLength=bufferPtr-buffer;
    copyLength=COINSPARK_MIN(bufferLength, stringMaxLen-1);
    memcpy(string, buffer, copyLength);
    string[copyLength]=0x00;
    
    return (copyLength==bufferLength);
}

bool CoinSparkPaymentRefIsValid(const CoinSparkPaymentRef paymentRef)
{
    return (paymentRef>=0) && (paymentRef<=COINSPARK_PAYMENT_REF_MAX);
}

CoinSparkPaymentRef CoinSparkPaymentRefRandom()
{
    CoinSparkPaymentRef paymentRef;
    long long bitsRemaining;
    
    paymentRef=0;
    
    for (bitsRemaining=COINSPARK_PAYMENT_REF_MAX; bitsRemaining>0; bitsRemaining>>=13) {
        paymentRef<<=13;
        paymentRef|=rand()&0x1FFF;
    }
    
    return paymentRef % (1+COINSPARK_PAYMENT_REF_MAX);
}

size_t CoinSparkPaymentRefEncode(const CoinSparkPaymentRef paymentRef, char* metadata, const size_t metadataMaxLen)
{
    long long paymentLeft;
    size_t bytes;
    char *metadataPtr, *metadataEnd;

    if (!CoinSparkPaymentRefIsValid(paymentRef))
        goto cannotEncodePaymentRef;

    metadataPtr=metadata;
    metadataEnd=metadataPtr+metadataMaxLen;

//  4-character identifier
    
    if ((metadataPtr+COINSPARK_METADATA_IDENTIFIER_LEN+1)<=metadataEnd) {
        memcpy(metadataPtr, COINSPARK_METADATA_IDENTIFIER, COINSPARK_METADATA_IDENTIFIER_LEN);
        metadataPtr+=COINSPARK_METADATA_IDENTIFIER_LEN;
        *metadataPtr++=COINSPARK_PAYMENTREF_PREFIX;

    } else
        goto cannotEncodePaymentRef;

//  The payment reference

    bytes=0;
    for (paymentLeft=paymentRef; paymentLeft>0; paymentLeft>>=8)
        bytes++;
    
    if ((metadataPtr+bytes)<=metadataEnd) {
        WriteSmallEndianUnsigned(paymentRef, metadataPtr, bytes);
        metadataPtr+=bytes;
        
    } else
        goto cannotEncodePaymentRef;
    
//  Return the number of bytes used
    
    return metadataPtr-metadata;
    
    cannotEncodePaymentRef:
    return 0;
}

bool CoinSparkPaymentRefDecode(CoinSparkPaymentRef* paymentRef, const char* metadata, const size_t metadataLen)
{
    const char *metadataPtr, *metadataEnd;
    size_t finalMetadataLen;
    
    metadataPtr=metadata;
    metadataEnd=metadataPtr+metadataLen;
    
    if (!LocateMetadataRange(&metadataPtr, &metadataEnd, COINSPARK_PAYMENTREF_PREFIX))
        goto cannotDecodePaymentRef;

//  The payment reference
    
    finalMetadataLen=metadataEnd-metadataPtr;
    if (finalMetadataLen>8)
        goto cannotDecodePaymentRef;
    
    *paymentRef=ReadSmallEndianUnsigned(metadataPtr, finalMetadataLen);

//  Return validity
    
    return CoinSparkPaymentRefIsValid(*paymentRef);
    
    cannotDecodePaymentRef:
    return FALSE;
}

size_t CoinSparkGenesisCalcAssetURL(const CoinSparkGenesis *genesis, const char* firstSpentTxID, const int firstSpentVout, char* urlString, const size_t urlStringMaxLen)
{
    char firstSpentTxIdPart[17], fullURL[256];
    int charIndex;
    size_t fullURLLen;
    
    for (charIndex=0; charIndex<16; charIndex++)
        firstSpentTxIdPart[charIndex]=firstSpentTxID[(charIndex+firstSpentVout)%64];
    firstSpentTxIdPart[16]=0; // C null string terminator
    
    sprintf(fullURL, "%s://%s/%s%s/", genesis->useHttps ? "https" : "http", genesis->domainName, genesis->usePrefix ? "coinspark/" : "",
            genesis->pagePath[0] ? genesis->pagePath : firstSpentTxIdPart);
    fullURLLen=strlen(fullURL);
    
    if (fullURLLen<urlStringMaxLen) { // allow for C null terminator
        for (charIndex=0; charIndex<=fullURLLen; charIndex++)
            urlString[charIndex]=tolower(fullURL[charIndex]);

        return fullURLLen;

    } else
        return 0;
}

void CoinSparkCalcAssetHash(const char* name, size_t nameLen,
                            const char* issuer, size_t issuerLen,
                            const char* description, size_t descriptionLen,
                            const char* units, size_t unitsLen,
                            const char* issueDate, size_t issueDateLen,
                            const char* expiryDate, size_t expiryDateLen,
                            const double* interestRate, const double* multiple,
                            const char* contractContent, const size_t contractContentLen,
                            unsigned char assetHash[32])
{
    char *buffer, *bufferPtr;
    long long interestRateToHash, multipleToHash;
    bool keepTrimming;
    
    buffer=malloc(nameLen+issuerLen+descriptionLen+issueDateLen+expiryDateLen+contractContentLen+1024);
    bufferPtr=buffer;
    
    #define TRIM_STRING(string, stringLen) \
        for (keepTrimming=TRUE; keepTrimming && (stringLen>0); ) \
            switch (string[0]) { \
                case 0x09: case 0x0A: case 0x0D: case 0x20: \
                    string++; \
                    stringLen--; \
                    break; \
                default: \
                    keepTrimming=FALSE; \
                    break; \
            } \
        for (keepTrimming=TRUE; keepTrimming && (stringLen>0); ) \
            switch (string[stringLen-1]) { \
                case 0x09: case 0x0A: case 0x0D: case 0x20: \
                    stringLen--; \
                    break; \
                default: \
                    keepTrimming=FALSE; \
                    break; \
            }
    
    
    #define ADD_HASH_BUFFER_STRING(string, stringLen) \
        if (string && stringLen) { \
            memcpy(bufferPtr, string, stringLen); \
            bufferPtr+=stringLen; \
        } \
        *bufferPtr++=0x00;
    
    TRIM_STRING(name, nameLen);
    TRIM_STRING(issuer, issuerLen);
    TRIM_STRING(description, descriptionLen);
    TRIM_STRING(units, unitsLen);
    TRIM_STRING(issueDate, issueDateLen);
    TRIM_STRING(expiryDate, expiryDateLen);

    ADD_HASH_BUFFER_STRING(name, nameLen);
    ADD_HASH_BUFFER_STRING(issuer, issuerLen);
    ADD_HASH_BUFFER_STRING(description, descriptionLen);
    ADD_HASH_BUFFER_STRING(units, unitsLen);
    ADD_HASH_BUFFER_STRING(issueDate, issueDateLen);
    ADD_HASH_BUFFER_STRING(expiryDate, expiryDateLen);
    
    interestRateToHash=(long long)((interestRate ? *interestRate : 0)*1000000.0+0.5);
    multipleToHash=(long long)((multiple ? *multiple : 1)*1000000.0+0.5);

    bufferPtr+=1+sprintf(bufferPtr, "%lld", (long long)interestRateToHash);
    bufferPtr+=1+sprintf(bufferPtr, "%lld", (long long)multipleToHash);
    
    ADD_HASH_BUFFER_STRING(contractContent, contractContentLen);
    
    CoinSparkCalcSHA256Hash((unsigned char*)buffer, bufferPtr-buffer, assetHash);
    
    free(buffer);
}

char* CoinSparkGetGenesisWebPageURL(const char* scriptPubKeys[], const size_t scriptPubKeysLen[], const int countOutputs, u_int8_t firstSpentTxId[32], const int firstSpentVout)
{
    char firstSpentTxIdString[65], metadata[400];
    size_t metadataLen;
    char* webPageURL;
    CoinSparkGenesis genesis;
    
    webPageURL=NULL;
   
    BinaryToHex(firstSpentTxId, 32, firstSpentTxIdString);
    
    metadataLen=CoinSparkScriptsToMetadata(scriptPubKeys, scriptPubKeysLen, FALSE, countOutputs, metadata, sizeof(metadata));
    if (!metadataLen)
        goto returnWebPageURL;
    
    if (!CoinSparkGenesisDecode(&genesis, metadata, metadataLen))
        goto returnWebPageURL;
    
    webPageURL=malloc(1024);
    CoinSparkGenesisCalcAssetURL(&genesis, firstSpentTxIdString, firstSpentVout, webPageURL, 1024);
    
    returnWebPageURL:
    return webPageURL; // called must call free() on the result but anyway this is for contract only
}

CoinSparkAssetQty CoinSparkGetGenesisOutputQty(const char* scriptPubKeys[], const size_t scriptPubKeysLen[],
                                               const CoinSparkSatoshiQty* outputsSatoshis, const int countOutputs,
                                               const CoinSparkSatoshiQty transactionFee, const int getOutputIndex)
{
    CoinSparkAssetQty outputQty, *outputBalances;
    char metadata[400];
    size_t metadataLen;
    CoinSparkGenesis genesis;
    CoinSparkSatoshiQty minValidFee;
    int outputIndex;
    bool *outputsRegular;
   
//  Default values for the end of this function
    
    outputQty=0;
    outputsRegular=NULL;
    outputBalances=NULL;

//  Decode the metadata

    metadataLen=CoinSparkScriptsToMetadata(scriptPubKeys, scriptPubKeysLen, FALSE, countOutputs, metadata, sizeof(metadata));
    if (!metadataLen)
        goto returnOutputQty;

    if (!CoinSparkGenesisDecode(&genesis, metadata, metadataLen))
        goto returnOutputQty;
    
//  Calculate outputsRegular flags

    outputsRegular=malloc(countOutputs*sizeof(*outputsRegular));
    
    for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
        outputsRegular[outputIndex]=CoinSparkScriptIsRegular(scriptPubKeys[outputIndex], scriptPubKeysLen[outputIndex], FALSE);

//  Check the transaction fee is sufficient
    
    minValidFee=CoinSparkGenesisCalcMinFee(&genesis, outputsSatoshis, outputsRegular, countOutputs);

    if (transactionFee<minValidFee)
        goto returnOutputQty;

//  Perform the genesis calculation and extract the relevant output for the calculation

    outputBalances=malloc(countOutputs*sizeof(*outputBalances));
    
    CoinSparkGenesisApply(&genesis, outputsRegular, outputBalances, countOutputs);
    
    outputQty=outputBalances[getOutputIndex];

//  Free memory and return result
    
    returnOutputQty:
        
    if (outputsRegular)
        free(outputsRegular);
    
    if (outputBalances)
        free(outputBalances);

    return outputQty;
}

CoinSparkAssetQty CoinSparkGetTransferOutputQty(const char* genesisScriptPubKeys[], const size_t genesisScriptPubKeysLen[],
                                                const CoinSparkSatoshiQty* genesisOutputsSatoshis, const int genesisCountOutputs,
                                                const CoinSparkSatoshiQty genesisTransactionFee,
                                                int64_t genesisBlockNum, int64_t genesisTxOffset, u_int8_t genesisTxId[32],
                                                const CoinSparkAssetQty* thisInputBalances, const int thisCountInputs,
                                                const char* thisScriptPubKeys[], const size_t thisScriptPubKeysLen[],
                                                const CoinSparkSatoshiQty* thisOutputsSatoshis, const int thisCountOutputs,
                                                const CoinSparkSatoshiQty thisTransactionFee, const int getOutputIndex)
{
    CoinSparkAssetQty outputQty, *thisOutputBalances;
    char genesisMetadata[400], thisMetadata[400];
    size_t genesisMetadataLen, thisMetadataLen;
    CoinSparkSatoshiQty genesisMinValidFee, thisMinValidFee;
    CoinSparkGenesis genesis;
    CoinSparkAssetRef assetRef;
    CoinSparkTransfer *transfers;
    int countTransfers, decodedTransfers, outputIndex;
    bool *genesisOutputsRegular, *thisOutputsRegular;

//  Default values for the end of this function
    
    outputQty=0;
    genesisOutputsRegular=NULL;
    transfers=NULL;
    thisOutputsRegular=NULL;
    thisOutputBalances=NULL;
    
//  Decode the metadata in the genesis transaction
    
    genesisMetadataLen=CoinSparkScriptsToMetadata(genesisScriptPubKeys, genesisScriptPubKeysLen, FALSE,
                                                  genesisCountOutputs, genesisMetadata, sizeof(genesisMetadata));
    
    if (!genesisMetadataLen)
        goto returnOutputQty;
    
    if (!CoinSparkGenesisDecode(&genesis, genesisMetadata, genesisMetadataLen))
        goto returnOutputQty;
    
//  Calculate outputsRegular flags for the genesis transaction
    
    genesisOutputsRegular=malloc(genesisCountOutputs*sizeof(*genesisOutputsRegular));
    
    for (outputIndex=0; outputIndex<genesisCountOutputs; outputIndex++)
        genesisOutputsRegular[outputIndex]=CoinSparkScriptIsRegular(genesisScriptPubKeys[outputIndex], genesisScriptPubKeysLen[outputIndex], FALSE);
    
//  Check the genesis transaction fee is sufficient
    
    genesisMinValidFee=CoinSparkGenesisCalcMinFee(&genesis, genesisOutputsSatoshis, genesisOutputsRegular, genesisCountOutputs);

    if (genesisTransactionFee<genesisMinValidFee)
        goto returnOutputQty;
    
//  Decode the metadata in this transaction
    
    thisMetadataLen=CoinSparkScriptsToMetadata(thisScriptPubKeys, thisScriptPubKeysLen, FALSE, thisCountOutputs, thisMetadata, sizeof(thisMetadata));
    
    if (thisMetadataLen)
        countTransfers=CoinSparkTransfersDecodeCount(thisMetadata, thisMetadataLen);
    else
        countTransfers=0;
    
    if (countTransfers>0) {
        transfers=malloc(countTransfers*sizeof(*transfers));
        decodedTransfers=CoinSparkTransfersDecode(transfers, countTransfers, thisCountInputs, thisCountOutputs, thisMetadata, thisMetadataLen);

        if (decodedTransfers!=countTransfers)
            goto returnOutputQty;
    }
    
//  Calculate outputsRegular flags for this transaction
    
    thisOutputsRegular=malloc(thisCountOutputs*sizeof(*thisOutputsRegular));
    
    for (outputIndex=0; outputIndex<thisCountOutputs; outputIndex++)
        thisOutputsRegular[outputIndex]=CoinSparkScriptIsRegular(thisScriptPubKeys[outputIndex], thisScriptPubKeysLen[outputIndex], FALSE);
    
//  Calculate the minimum transaction fee for the transfer list to be valid
    
    thisMinValidFee=CoinSparkTransfersCalcMinFee(transfers, countTransfers, thisCountInputs, thisCountOutputs, thisOutputsSatoshis, thisOutputsRegular);
    
//  Built the asset reference
    
    assetRef.blockNum=genesisBlockNum;
    assetRef.txOffset=genesisTxOffset;
    memcpy(assetRef.txIDPrefix, genesisTxId, COINSPARK_ASSETREF_TXID_PREFIX_LEN);
    
//  Perform the transfer calculation

    thisOutputBalances=malloc(thisCountOutputs*sizeof(*thisOutputBalances));
    
    if (thisTransactionFee>=thisMinValidFee)
        CoinSparkTransfersApply(&assetRef, &genesis, transfers, countTransfers, thisInputBalances, thisCountInputs,
                                thisOutputsRegular, thisOutputBalances, thisCountOutputs);
    else
        CoinSparkTransfersApplyNone(&assetRef, &genesis, thisInputBalances, thisCountInputs, thisOutputsRegular, thisOutputBalances, thisCountOutputs);
    
//  Extract the relevant output for the calculation
    
    outputQty=thisOutputBalances[getOutputIndex];
    
//  Free memory and return result
    
    returnOutputQty:
        
    if (genesisOutputsRegular)
        free(genesisOutputsRegular);
    
    if (transfers)
        free(transfers);
    
    if (thisOutputsRegular)
        free(thisOutputsRegular);
    
    if (thisOutputBalances)
        free(thisOutputBalances);
    
    return outputQty;
}

//	Code below is adapted from Dr Brian Gladman's library via http://www.gladman.me.uk/

/*
 ---------------------------------------------------------------------------
 Copyright (c) 2002, Dr Brian Gladman, Worcester, UK.   All rights reserved.
 
 LICENSE TERMS
 
 The free distribution and use of this software in both source and binary
 form is allowed (with or without changes) provided that:
 
 1. distributions of this source code include the above copyright
 notice, this list of conditions and the following disclaimer;
 
 2. distributions in binary form include the above copyright
 notice, this list of conditions and the following disclaimer
 in the documentation and/or other associated materials;
 
 3. the copyright holder's name is not used to endorse products
 built using this software without specific written permission.
 
 ALTERNATIVELY, provided that this notice is retained in full, this product
 may be distributed under the terms of the GNU General Public License (GPL),
 in which case the provisions of the GPL apply INSTEAD OF those given above.
 
 DISCLAIMER
 
 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 Issue Date: 01/08/2005
 */

/******************** brg_endian.h ********************/

#define IS_BIG_ENDIAN      4321 /* byte 0 is most significant (mc68k) */
#define IS_LITTLE_ENDIAN   1234 /* byte 0 is least significant (i386) */

/* Include files where endian defines and byteswap functions may reside */
#if defined( __FreeBSD__ ) || defined( __OpenBSD__ ) || defined( __NetBSD__ )
#  include <sys/endian.h>
#elif defined( BSD ) && ( BSD >= 199103 ) || defined( __APPLE__ ) || \
defined( __CYGWIN32__ ) || defined( __DJGPP__ ) || defined( __osf__ )
#  include <machine/endian.h>
#elif defined( __linux__ ) || defined( __GNUC__ ) || defined( __GNU_LIBRARY__ )
#  if !defined( __MINGW32__ )
#    include <endian.h>
#    if !defined( __BEOS__ )
#      include <byteswap.h>
#    endif
#  endif
#endif

/* Now attempt to set the define for platform byte order using any  */
/* of the four forms SYMBOL, _SYMBOL, __SYMBOL & __SYMBOL__, which  */
/* seem to encompass most endian symbol definitions                 */

#if defined( BIG_ENDIAN ) && defined( LITTLE_ENDIAN )
#  if defined( BYTE_ORDER ) && BYTE_ORDER == BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( BYTE_ORDER ) && BYTE_ORDER == LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( _BIG_ENDIAN ) && defined( _LITTLE_ENDIAN )
#  if defined( _BYTE_ORDER ) && _BYTE_ORDER == _BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( _BYTE_ORDER ) && _BYTE_ORDER == _LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( _BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( _LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( __BIG_ENDIAN ) && defined( __LITTLE_ENDIAN )
#  if defined( __BYTE_ORDER ) && __BYTE_ORDER == __BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( __BYTE_ORDER ) && __BYTE_ORDER == __LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( __BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( __LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( __BIG_ENDIAN__ ) && defined( __LITTLE_ENDIAN__ )
#  if defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __BIG_ENDIAN__
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __LITTLE_ENDIAN__
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( __BIG_ENDIAN__ )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( __LITTLE_ENDIAN__ )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

/*  if the platform byte order could not be determined, then try to */
/*  set this define using common machine defines                    */
#if !defined(PLATFORM_BYTE_ORDER)

#if   defined( __alpha__ ) || defined( __alpha ) || defined( i386 )       || \
defined( __i386__ )  || defined( _M_I86 )  || defined( _M_IX86 )    || \
defined( __OS2__ )   || defined( sun386 )  || defined( __TURBOC__ ) || \
defined( vax )       || defined( vms )     || defined( VMS )        || \
defined( __VMS )     || defined( _M_X64 )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN

#elif defined( AMIGA )   || defined( applec )    || defined( __AS400__ )  || \
defined( _CRAY )   || defined( __hppa )    || defined( __hp9000 )   || \
defined( ibm370 )  || defined( mc68000 )   || defined( m68k )       || \
defined( __MRC__ ) || defined( __MVS__ )   || defined( __MWERKS__ ) || \
defined( sparc )   || defined( __sparc)    || defined( SYMANTEC_C ) || \
defined( __VOS__ ) || defined( __TIGCC__ ) || defined( __TANDEM )   || \
defined( THINK_C ) || defined( __VMCMS__ )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN

#elif 0     /* **** EDIT HERE IF NECESSARY **** */
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#elif 0     /* **** EDIT HERE IF NECESSARY **** */
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#else
#  error Please edit lines 126 or 128 in brg_endian.h to set the platform byte order
#endif

#endif

/******************** brg_types.h ********************/

#include <limits.h>
    
#ifndef BRG_UI8
#  define BRG_UI8
#  if UCHAR_MAX == 255u
    typedef unsigned char uint_8t;
#  else
#    error Please define uint_8t as an 8-bit unsigned integer type in brg_types.h
#  endif
#endif
    
#ifndef BRG_UI16
#  define BRG_UI16
#  if USHRT_MAX == 65535u
    typedef unsigned short uint_16t;
#  else
#    error Please define uint_16t as a 16-bit unsigned short type in brg_types.h
#  endif
#endif
    
#ifndef BRG_UI32
#  define BRG_UI32
#  if UINT_MAX == 4294967295u
#    define li_32(h) 0x##h##u
    typedef unsigned int uint_32t;
#  elif ULONG_MAX == 4294967295u
#    define li_32(h) 0x##h##ul
    typedef unsigned long uint_32t;
#  elif defined( _CRAY )
#    error This code needs 32-bit data types, which Cray machines do not provide
#  else
#    error Please define uint_32t as a 32-bit unsigned integer type in brg_types.h
#  endif
#endif
    
#ifndef BRG_UI64
#  if defined( __BORLANDC__ ) && !defined( __MSDOS__ )
#    define BRG_UI64
#    define li_64(h) 0x##h##ull
    typedef unsigned __int64 uint_64t;
#  elif defined( _MSC_VER ) && ( _MSC_VER < 1300 )    /* 1300 == VC++ 7.0 */
#    define BRG_UI64
#    define li_64(h) 0x##h##ui64
    typedef unsigned __int64 uint_64t;
#  elif defined( __sun ) && defined(ULONG_MAX) && ULONG_MAX == 0xfffffffful
#    define BRG_UI64
#    define li_64(h) 0x##h##ull
    typedef unsigned long long uint_64t;
#  elif defined( UINT_MAX ) && UINT_MAX > 4294967295u
#    if UINT_MAX == 18446744073709551615u
#      define BRG_UI64
#      define li_64(h) 0x##h##u
    typedef unsigned int uint_64t;
#    endif
#  elif defined( ULONG_MAX ) && ULONG_MAX > 4294967295u
#    if ULONG_MAX == 18446744073709551615ul
#      define BRG_UI64
#      define li_64(h) 0x##h##ul
    typedef unsigned long uint_64t;
#    endif
#  elif defined( ULLONG_MAX ) && ULLONG_MAX > 4294967295u
#    if ULLONG_MAX == 18446744073709551615ull
#      define BRG_UI64
#      define li_64(h) 0x##h##ull
    typedef unsigned long long uint_64t;
#    endif
#  elif defined( ULONG_LONG_MAX ) && ULONG_LONG_MAX > 4294967295u
#    if ULONG_LONG_MAX == 18446744073709551615ull
#      define BRG_UI64
#      define li_64(h) 0x##h##ull
    typedef unsigned long long uint_64t;
#    endif
#  endif
#endif
    
#if defined( NEED_UINT_64T ) && !defined( BRG_UI64 )
#  error Please define uint_64t as an unsigned 64 bit type in brg_types.h
#endif
    
#ifndef RETURN_VALUES
#  define RETURN_VALUES
#  if defined( DLL_EXPORT )
#    if defined( _MSC_VER ) || defined ( __INTEL_COMPILER )
#      define VOID_RETURN    __declspec( dllexport ) void __stdcall
#      define INT_RETURN     __declspec( dllexport ) int  __stdcall
#    elif defined( __GNUC__ )
#      define VOID_RETURN    __declspec( __dllexport__ ) void
#      define INT_RETURN     __declspec( __dllexport__ ) int
#    else
#      error Use of the DLL is only available on the Microsoft, Intel and GCC compilers
#    endif
#  elif defined( DLL_IMPORT )
#    if defined( _MSC_VER ) || defined ( __INTEL_COMPILER )
#      define VOID_RETURN    __declspec( dllimport ) void __stdcall
#      define INT_RETURN     __declspec( dllimport ) int  __stdcall
#    elif defined( __GNUC__ )
#      define VOID_RETURN    __declspec( __dllimport__ ) void
#      define INT_RETURN     __declspec( __dllimport__ ) int
#    else
#      error Use of the DLL is only available on the Microsoft, Intel and GCC compilers
#    endif
#  elif defined( __WATCOMC__ )
#    define VOID_RETURN  void __cdecl
#    define INT_RETURN   int  __cdecl
#  else
#    define VOID_RETURN  void
#    define INT_RETURN   int
#  endif
#endif
    
#define ui_type(size)               uint_##size##t
#define dec_unit_type(size,x)       typedef ui_type(size) x
#define dec_bufr_type(size,bsize,x) typedef ui_type(size) x[bsize / (size >> 3)]
#define ptr_cast(x,size)            ((ui_type(size)*)(x))

/******************** sha2.h ********************/

#define SHA_256

#define SHA256_DIGEST_SIZE  32
#define SHA256_BLOCK_SIZE   64
    
    typedef struct
    {   uint_32t count[2];
        uint_32t hash[8];
        uint_32t wbuf[16];
    } sha256_ctx;
    
    static VOID_RETURN sha256_compile(sha256_ctx ctx[1]);
    
    static VOID_RETURN sha256_begin(sha256_ctx ctx[1]);
    static VOID_RETURN sha256_hash(const unsigned char data[], unsigned long len, sha256_ctx ctx[1]);
    static VOID_RETURN sha256(unsigned char hval[], const unsigned char data[], unsigned long len);

/******************** sha2.c ********************/

#if defined( _MSC_VER ) && ( _MSC_VER > 800 )
#pragma intrinsic(memcpy)
#endif
    
#define rotl32(x,n)   (((x) << n) | ((x) >> (32 - n)))
#define rotr32(x,n)   (((x) >> n) | ((x) << (32 - n)))

#if !defined(bswap_32)
#define bswap_32(x) ((rotr32((x), 24) & 0x00ff00ff) | (rotr32((x), 8) & 0xff00ff00))
#endif
    
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#define SWAP_BYTES
#else
#undef  SWAP_BYTES
#endif
    
#define ch(x,y,z)       ((z) ^ ((x) & ((y) ^ (z))))
#define maj(x,y,z)      (((x) & (y)) | ((z) & ((x) ^ (y))))
    
    /* round transforms for SHA256 and SHA512 compression functions */
    
#define vf(n,i) v[(n - i) & 7]
    
#define hf(i) (p[i & 15] += \
g_1(p[(i + 14) & 15]) + p[(i + 9) & 15] + g_0(p[(i + 1) & 15]))
    
#define v_cycle(i,j)                                \
vf(7,i) += (j ? hf(i) : p[i]) + k_0[i+j]        \
+ s_1(vf(4,i)) + ch(vf(4,i),vf(5,i),vf(6,i));   \
vf(3,i) += vf(7,i);                             \
vf(7,i) += s_0(vf(0,i))+ maj(vf(0,i),vf(1,i),vf(2,i))
    
#if defined(SHA_224) || defined(SHA_256)
    
#define SHA256_MASK (SHA256_BLOCK_SIZE - 1)
    
#if defined(SWAP_BYTES)
#define bsw_32(p,n) \
{ int _i = (n); while(_i--) ((uint_32t*)p)[_i] = bswap_32(((uint_32t*)p)[_i]); }
#else
#define bsw_32(p,n)
#endif
    
#define s_0(x)  (rotr32((x),  2) ^ rotr32((x), 13) ^ rotr32((x), 22))
#define s_1(x)  (rotr32((x),  6) ^ rotr32((x), 11) ^ rotr32((x), 25))
#define g_0(x)  (rotr32((x),  7) ^ rotr32((x), 18) ^ ((x) >>  3))
#define g_1(x)  (rotr32((x), 17) ^ rotr32((x), 19) ^ ((x) >> 10))
#define k_0     k256
    
    /* rotated SHA256 round definition. Rather than swapping variables as in    */
    /* FIPS-180, different variables are 'rotated' on each round, returning     */
    /* to their starting positions every eight rounds                           */
    
#define q(n)  v##n
    
#define one_cycle(a,b,c,d,e,f,g,h,k,w)  \
q(h) += s_1(q(e)) + ch(q(e), q(f), q(g)) + k + w; \
q(d) += q(h); q(h) += s_0(q(a)) + maj(q(a), q(b), q(c))
    
    /* SHA256 mixing data   */
    
    static const uint_32t k256[64] =
    {   0x428a2f98ul, 0x71374491ul, 0xb5c0fbcful, 0xe9b5dba5ul,
        0x3956c25bul, 0x59f111f1ul, 0x923f82a4ul, 0xab1c5ed5ul,
        0xd807aa98ul, 0x12835b01ul, 0x243185beul, 0x550c7dc3ul,
        0x72be5d74ul, 0x80deb1feul, 0x9bdc06a7ul, 0xc19bf174ul,
        0xe49b69c1ul, 0xefbe4786ul, 0x0fc19dc6ul, 0x240ca1ccul,
        0x2de92c6ful, 0x4a7484aaul, 0x5cb0a9dcul, 0x76f988daul,
        0x983e5152ul, 0xa831c66dul, 0xb00327c8ul, 0xbf597fc7ul,
        0xc6e00bf3ul, 0xd5a79147ul, 0x06ca6351ul, 0x14292967ul,
        0x27b70a85ul, 0x2e1b2138ul, 0x4d2c6dfcul, 0x53380d13ul,
        0x650a7354ul, 0x766a0abbul, 0x81c2c92eul, 0x92722c85ul,
        0xa2bfe8a1ul, 0xa81a664bul, 0xc24b8b70ul, 0xc76c51a3ul,
        0xd192e819ul, 0xd6990624ul, 0xf40e3585ul, 0x106aa070ul,
        0x19a4c116ul, 0x1e376c08ul, 0x2748774cul, 0x34b0bcb5ul,
        0x391c0cb3ul, 0x4ed8aa4aul, 0x5b9cca4ful, 0x682e6ff3ul,
        0x748f82eeul, 0x78a5636ful, 0x84c87814ul, 0x8cc70208ul,
        0x90befffaul, 0xa4506cebul, 0xbef9a3f7ul, 0xc67178f2ul,
    };
    
    /* Compile 64 bytes of hash data into SHA256 digest value   */
    /* NOTE: this routine assumes that the byte order in the    */
    /* ctx->wbuf[] at this point is such that low address bytes */
    /* in the ORIGINAL byte stream will go into the high end of */
    /* words on BOTH big and little endian systems              */
    
    static VOID_RETURN sha256_compile(sha256_ctx ctx[1])
    {
        uint_32t j, *p = ctx->wbuf, v[8];
        
        memcpy(v, ctx->hash, 8 * sizeof(uint_32t));
        
        for(j = 0; j < 64; j += 16)
        {
            v_cycle( 0, j); v_cycle( 1, j);
            v_cycle( 2, j); v_cycle( 3, j);
            v_cycle( 4, j); v_cycle( 5, j);
            v_cycle( 6, j); v_cycle( 7, j);
            v_cycle( 8, j); v_cycle( 9, j);
            v_cycle(10, j); v_cycle(11, j);
            v_cycle(12, j); v_cycle(13, j);
            v_cycle(14, j); v_cycle(15, j);
        }
        
        ctx->hash[0] += v[0]; ctx->hash[1] += v[1];
        ctx->hash[2] += v[2]; ctx->hash[3] += v[3];
        ctx->hash[4] += v[4]; ctx->hash[5] += v[5];
        ctx->hash[6] += v[6]; ctx->hash[7] += v[7];
    }
    
    /* SHA256 hash data in an array of bytes into hash buffer   */
    /* and call the hash_compile function as required.          */
    
    static VOID_RETURN sha256_hash(const unsigned char data[], unsigned long len, sha256_ctx ctx[1])
    {   uint_32t pos = (uint_32t)(ctx->count[0] & SHA256_MASK),
        space = SHA256_BLOCK_SIZE - pos;
        const unsigned char *sp = data;
        
        if((ctx->count[0] += len) < len)
            ++(ctx->count[1]);
        
        while(len >= space)     /* tranfer whole blocks while possible  */
        {
            memcpy(((unsigned char*)ctx->wbuf) + pos, sp, space);
            sp += space; len -= space; space = SHA256_BLOCK_SIZE; pos = 0;
            bsw_32(ctx->wbuf, SHA256_BLOCK_SIZE >> 2)
            sha256_compile(ctx);
        }
        
        memcpy(((unsigned char*)ctx->wbuf) + pos, sp, len);
    }
    
    /* SHA256 Final padding and digest calculation  */
    
    static void sha_end1(unsigned char hval[], sha256_ctx ctx[1], const unsigned int hlen)
    {   uint_32t    i = (uint_32t)(ctx->count[0] & SHA256_MASK);
        
        /* put bytes in the buffer in an order in which references to   */
        /* 32-bit words will put bytes with lower addresses into the    */
        /* top of 32 bit words on BOTH big and little endian machines   */
        bsw_32(ctx->wbuf, (i + 3) >> 2)
        
        /* we now need to mask valid bytes and add the padding which is */
        /* a single 1 bit and as many zero bits as necessary. Note that */
        /* we can always add the first padding byte here because the    */
        /* buffer always has at least one empty slot                    */
        ctx->wbuf[i >> 2] &= 0xffffff80 << 8 * (~i & 3);
        ctx->wbuf[i >> 2] |= 0x00000080 << 8 * (~i & 3);
        
        /* we need 9 or more empty positions, one for the padding byte  */
        /* (above) and eight for the length count.  If there is not     */
        /* enough space pad and empty the buffer                        */
        if(i > SHA256_BLOCK_SIZE - 9)
        {
            if(i < 60) ctx->wbuf[15] = 0;
            sha256_compile(ctx);
            i = 0;
        }
        else    /* compute a word index for the empty buffer positions  */
            i = (i >> 2) + 1;
        
        while(i < 14) /* and zero pad all but last two positions        */
            ctx->wbuf[i++] = 0;
        
        /* the following 32-bit length fields are assembled in the      */
        /* wrong byte order on little endian machines but this is       */
        /* corrected later since they are only ever used as 32-bit      */
        /* word values.                                                 */
        ctx->wbuf[14] = (ctx->count[1] << 3) | (ctx->count[0] >> 29);
        ctx->wbuf[15] = ctx->count[0] << 3;
        sha256_compile(ctx);
        
        /* extract the hash value as bytes in case the hash buffer is   */
        /* mislaigned for 32-bit words                                  */
        for(i = 0; i < hlen; ++i)
            hval[i] = (unsigned char)(ctx->hash[i >> 2] >> (8 * (~i & 3)));
    }
    
#endif
    
#if defined(SHA_256)
    
    static const uint_32t i256[8] =
    {
        0x6a09e667ul, 0xbb67ae85ul, 0x3c6ef372ul, 0xa54ff53aul,
        0x510e527ful, 0x9b05688cul, 0x1f83d9abul, 0x5be0cd19ul
    };
    
    static VOID_RETURN sha256_begin(sha256_ctx ctx[1])
    {
        ctx->count[0] = ctx->count[1] = 0;
        memcpy(ctx->hash, i256, 8 * sizeof(uint_32t));
    }
    
    static VOID_RETURN sha256(unsigned char hval[], const unsigned char data[], unsigned long len)
    {   sha256_ctx  cx[1];
        
        sha256_begin(cx);
        sha256_hash(data, len, cx);
        sha_end1(hval, cx, SHA256_DIGEST_SIZE);
    }
    
#endif
