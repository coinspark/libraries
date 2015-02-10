/*
 * CoinSpark 2.1 - C test suite
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


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#include "coinspark.h"
#include <openssl/sha.h>
#include <ctype.h>

#define MAX_OP_RETURN_LEN 40
#define MAX_TRANSFERS 10
#define MAX_INPUTS 5
#define MAX_OUTPUTS 10
#define MAX_MESSAGE_PARTS 5


void DisplayHexadecimal(const void* hex, size_t length, FILE* file)
{
    int index;
    
    for (index=0; index<length; index++)
        fprintf(file, "%02X", ((unsigned char*)hex)[index]);
}

u_int64_t BigRandomNumber()
{
    return ((u_int64_t)rand())*RAND_MAX+(u_int64_t)rand();
}

void RandomizeRawData(void* data, size_t size)
{
    int index;
    
    for (index=0; index<size; index++)
        ((unsigned char*)data)[index]=rand()%0xFF;
}

CoinSparkAssetQty RandomizeAssetQty(const CoinSparkTransfer* previousTransfer)
{
    CoinSparkAssetQty quantity;
    int exponent;
    
    switch (rand()%8) {
        case 0:
            if (previousTransfer)
                quantity=previousTransfer->qtyPerOutput;
            else
                quantity=1;
            break;
            
        case 1:
            quantity=rand()&0xFF;
            break;
            
        case 2:
            quantity=rand()&0xFFFF;
            break;
            
        case 3:
            quantity=rand()&0xFFFFFF;
            break;
            
        case 4:
            quantity=rand()&0xFFFFFFFF;
            break;
            
        case 5:
            quantity=BigRandomNumber()&0xFFFFFFFFFFFF;
            break;
            
        case 6:
            quantity=1+rand()%1000;
            for (exponent=rand()%12; exponent>0; exponent--)
                quantity*=10;
            break;
            
        case 7:
            quantity=COINSPARK_ASSET_QTY_MAX;
            break;
    }
    
    return COINSPARK_MIN(quantity, COINSPARK_ASSET_QTY_MAX);
}

void DisplayRawData(void* data, size_t size, FILE* file)
{
    int rowSize=8, row, col;
    unsigned char ch, bit;
    
    fprintf(file, "RAW DATA\n");
    
    for (row=0; row<size; row+=rowSize) {
        fprintf(file, "%3d  ", row);
        
        for (col=row; col<(row+rowSize); col++) {
            ch=(col<size) ? ((unsigned char*)data)[col] : ' ';
           
            if ((ch<32) || (ch>=127))
                ch='?';
            fputc(ch, file);
        }
        
        fprintf(file, "   ");
        
        for (col=row; col<(row+rowSize); col++) {
            if (col<size)
                fprintf(file, "%02X ", ((unsigned char*)data)[col]);
            else
                fprintf(file, "   ");
        }
        
        fprintf(file, "  ");
        
        for (col=row; col<(row+rowSize); col++) {
            if (col<size) {
                for (bit=0x80; bit; bit>>=1)
                    fputc((((unsigned char*)data)[col]&bit) ? '1' : '0', file);
            } else
                fprintf(file, "        ");
            
            fprintf(file, " ");
        }
        
        
        fprintf(file, "\n");
    }
    
    fprintf(file, "END RAW DATA\n\n");
}

void RandomizeDomainPath(char* domain, size_t domainMaxLen, int percentIPAddress, char* path, size_t pathMaxLen)
{
    int domainPos, pathPos;
    size_t domainLen, pathLen;
    const char domainPathChars[]="0123456789abcdefghijklmnopqrstuvwxyz-";
    const char* domainSuffixes[]={
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
    
    if ((rand()%100)<percentIPAddress)
        sprintf(domain, "%d.%d.%d.%d", rand()%256, rand()%256, rand()%256, rand()%256);
    
    else {
        if (rand()%2)
            strcpy(domain, "www.");
        else
            *domain=0;
        
        domainPos=(int)strlen(domain);
        domainLen=domainPos+2+rand()%(domainMaxLen-12);
        for (; domainPos<domainLen; domainPos++)
            domain[domainPos]=domainPathChars[rand()%(sizeof(domainPathChars)-1)];
        domain[domainLen]=0x00;
        
        if (rand()%3)
            strcat(domain, domainSuffixes[rand()%(sizeof(domainSuffixes)/sizeof(*domainSuffixes))]);
    }

    pathLen=rand()%(1+pathMaxLen);
    for (pathPos=0; pathPos<pathLen; pathPos++)
        path[pathPos]=domainPathChars[rand()%(sizeof(domainPathChars)-1)];
    path[pathLen]=0x00;
}

void RandomizeGenesis(CoinSparkGenesis* genesis)
{
    CoinSparkGenesisClear(genesis);
    
    genesis->qtyMantissa=1+rand()%COINSPARK_GENESIS_QTY_MANTISSA_MAX;
    genesis->qtyExponent=rand()%(1+COINSPARK_GENESIS_QTY_EXPONENT_MAX);
    
    if (rand()%2) {
        genesis->chargeFlatExponent=rand()%(1+COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX);
        
        if (genesis->chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX)
            genesis->chargeFlatMantissa=1+rand()%COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX;
        else
            genesis->chargeFlatMantissa=1+rand()%COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX;
        
    } else {
        genesis->chargeFlatExponent=0;
        genesis->chargeFlatMantissa=0;
    }
    
    if (rand()%2)
        genesis->chargeBasisPoints=1+rand()%COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX;
    else
        genesis->chargeBasisPoints=0;
    
tryDomainPathAgain:
    
    genesis->useHttps=(rand()%2) ? TRUE : FALSE;
    genesis->usePrefix=(rand()%2) ? TRUE : FALSE;
    
    RandomizeDomainPath(genesis->domainName, COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN, 10, genesis->pagePath, COINSPARK_GENESIS_PAGE_PATH_MAX_LEN);
    
    genesis->assetHashLen=CoinSparkGenesisCalcHashLen(genesis, MAX_OP_RETURN_LEN);
    
    if (genesis->assetHashLen<COINSPARK_GENESIS_HASH_MIN_LEN)
        goto tryDomainPathAgain;
    
    RandomizeRawData(genesis->assetHash, genesis->assetHashLen);
}

size_t RandomizeBitcoinAddress(char* address, size_t bytesAvailable)
{
    size_t addressSize;
    int charIndex;
    const char base58Chars[]="123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    
    addressSize=bytesAvailable/2+rand()%(bytesAvailable/2)-1;
    
    for (charIndex=1; charIndex<addressSize; charIndex++)
        address[charIndex]=base58Chars[rand()%(sizeof(base58Chars)-1)];
    
    address[0]='1'; // make it look like a bitcoin address (even though it's almost definitely invalid)
    address[addressSize]=0; // C terminator byte
    
    return addressSize;
}

void RandomizeAddress(CoinSparkAddress* address)
{
    CoinSparkAddressClear(address);
    RandomizeBitcoinAddress(address->bitcoinAddress, 40);
    address->addressFlags=rand()&(COINSPARK_ADDRESS_FLAG_MASK>>(rand()%32));
//    address->addressFlags=COINSPARK_ADDRESS_FLAG_ASSETS|COINSPARK_ADDRESS_FLAG_PAYMENT_REFS;
    address->paymentRef=(rand()%2) ? CoinSparkPaymentRefRandom() : 0;
}

void RandomizeAssetRef(CoinSparkAssetRef* assetRef)
{
    CoinSparkAssetRefClear(assetRef);
    assetRef->blockNum=BigRandomNumber()%((rand()%2) ? COINSPARK_ASSETREF_BLOCK_NUM_MAX : 0xFFFFFF);
    assetRef->txOffset=BigRandomNumber()%((rand()%2) ? COINSPARK_ASSETREF_TX_OFFSET_MAX : 0xFFFFFF);
    RandomizeRawData(assetRef->txIDPrefix, sizeof(assetRef->txIDPrefix));
}

void RandomizeInOutRange(CoinSparkIORange* range, const CoinSparkIORange* previousRange, int countInputsOutputs)
{
    int version=rand()%9;
    
    switch (version) {
        case 0: // _0P
            if (previousRange)
                *range=*previousRange;
            else {
                range->first=0;
                range->count=1;
            }
            break;
            
        case 1: // _1S
            if (previousRange)
                range->first=previousRange->first+previousRange->count;
            else
                range->first=1;
            
            range->count=1;
            break;
            
        case 2: // _ALL
            range->first=0;
            range->count=countInputsOutputs;
            break;
            
        case 3: // _1_0_BYTE
            range->first=rand() % COINSPARK_MIN(0xFF, countInputsOutputs);
            range->count=1;
            break;
            
        case 4: // _0_1_BYTE
            range->first=0;
            range->count=rand() % COINSPARK_MIN(0xFF, countInputsOutputs);
            break;
            
        case 5: // _2_0_BYTES
            range->first=rand() % COINSPARK_MIN(0xFFFF, countInputsOutputs);
            range->count=1;
            break;
            
        case 6: // _1_1_BYTES
            range->first=rand() % COINSPARK_MIN(0xFF, countInputsOutputs);
            range->count=rand() % COINSPARK_MIN(0xFF, countInputsOutputs-range->first);
            break;
            
        case 7: // _2_1_BYTES
            range->first=rand() % COINSPARK_MIN(0xFFFF, countInputsOutputs);
            range->count=rand() % COINSPARK_MIN(0xFF, countInputsOutputs-range->first);
            break;
            
        case 8: // _2_2_BYTES
            range->first=rand() % COINSPARK_MIN(0xFFFF, countInputsOutputs);
            range->count=rand() % COINSPARK_MIN(0xFFFF, countInputsOutputs-range->first);
            break;
    }
    
    if (range->count<1)
        range->count=1;
    
    if (range->count>countInputsOutputs)
        range->count=countInputsOutputs;
    
    if (range->first>(countInputsOutputs-range->count))
        range->first=countInputsOutputs-range->count;
}

void RandomizeTransfer(CoinSparkTransfer* transfer, const CoinSparkTransfer* previousTransfer, int percentDefaultRoute, int countInputs, int countOutputs)
{
    CoinSparkTransferClear(transfer);
    
    if ((rand()%100)<percentDefaultRoute) // make it a default route
        transfer->assetRef.blockNum=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE;
    else if (previousTransfer && (rand()&1) ) // use same as previous
        transfer->assetRef=previousTransfer->assetRef;
    else
        RandomizeAssetRef(&transfer->assetRef);
    
    RandomizeInOutRange(&transfer->inputs, previousTransfer ? &previousTransfer->inputs : NULL, countInputs);
    RandomizeInOutRange(&transfer->outputs, previousTransfer ? &previousTransfer->outputs : NULL, countOutputs);
    
    if (transfer->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) {
        transfer->outputs.count=1;
        transfer->qtyPerOutput=COINSPARK_ASSET_QTY_MAX;
        
    } else
        transfer->qtyPerOutput=RandomizeAssetQty(previousTransfer);
}

bool RandomizeMessage(CoinSparkMessage* message, int countOutputs, size_t metadataMaxLen)
{ // returns whether successful or not, given space provided
    
    int outputRangeIndex, minOutputRanges;
    
    CoinSparkMessageClear(message);
    
    message->useHttps=(rand()%2) ? TRUE : FALSE;
    message->usePrefix=(rand()%2) ? TRUE : FALSE;
    message->isPublic=(rand()%2) ? TRUE : FALSE;
    minOutputRanges=message->isPublic ? 0 : 1;
    
    RandomizeDomainPath(message->serverHost, COINSPARK_MESSAGE_SERVER_HOST_MAX_LEN, 50, message->serverPath, COINSPARK_MESSAGE_SERVER_PATH_MAX_LEN);
    
    message->countOutputRanges=minOutputRanges+rand()%COINSPARK_MESSAGE_MAX_IO_RANGES;
    
    for (outputRangeIndex=0; outputRangeIndex<message->countOutputRanges; outputRangeIndex++)
        RandomizeInOutRange(message->outputRanges+outputRangeIndex, NULL, countOutputs);
    
    RandomizeRawData(message->hash, COINSPARK_MESSAGE_HASH_MAX_LEN);

    for (; message->countOutputRanges>=minOutputRanges; message->countOutputRanges--) {
        message->hashLen=CoinSparkMessageCalcHashLen(message, countOutputs, metadataMaxLen);
        if (message->hashLen>=COINSPARK_MESSAGE_HASH_MIN_LEN)
            return TRUE;
    }
    
    return FALSE;
}

void RandomizeReadableString(char* string, size_t stringMaxLen, bool padding)
{
    size_t length;
    int charIndex;
    char useChars[]=" .0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    length=2+rand()%(stringMaxLen-2); // will leave one byte for terminator

    for (charIndex=0; charIndex<length; charIndex++)
        if (padding && rand()%2)
            string[charIndex]=' ';
        else
            string[charIndex]=useChars[rand()%(sizeof(useChars)-1)];
    
    string[length]=0x00; // null terminator
}

size_t RandomizeRegularScript(char* scriptPubKey, size_t scriptPubKeyMaxLen)
{
    size_t scriptPubKeyLen;
    
    do {
        scriptPubKeyLen=1+rand()%scriptPubKeyMaxLen;
        RandomizeRawData(scriptPubKey, scriptPubKeyLen);
    } while
        (!CoinSparkScriptIsRegular(scriptPubKey, scriptPubKeyLen, FALSE));
    
    return scriptPubKeyLen;
}

void DisturbRawData(void* data, size_t size)
{
    if (size>0)
        ((unsigned char*)data)[rand()%size]^=1<<(rand()%8);
}

void DisturbString(char* string)
{
    size_t length;
    unsigned char bit;
    
    length=strlen(string);
    if (length>0) {
        bit=1<<(rand()%8);
        while (bit==32) // prevent transformation of lower to upper case
            bit|=1<<(rand()%8);
        ((unsigned char*)string)[rand()%length]^=bit;
    }
}

void DisturbIORange(CoinSparkIORange* range)
{
    if (rand()%2)
        range->first^=1;
    else
        range->count^=1;
}

void DisturbInteger16(int16_t* integer)
{
    *integer^=1<<(rand()%16);
}

void DisturbInteger32(int32_t* integer)
{
    *integer^=1<<(rand()%32);
}

void DisturbInteger64(int64_t* integer)
{
    *integer^=1<<(rand()%64);
}

bool StartTests(FILE** inputFile, FILE** outputFile, const char* directoryName, const char* testName, int countTests)
{
    char inputFilePath[1024], outputFilePath[1024];
    
    printf("%d %s tests starting...\n", countTests, testName);

    if (directoryName) {
        sprintf(inputFilePath, "%s/%s-Input.txt", directoryName, testName);
        sprintf(outputFilePath, "%s/%s-Output-C.txt", directoryName, testName);
        
        *inputFile=fopen(inputFilePath, "w");
        if (!*inputFile) {
            printf("Could not open for writing: %s\n\n", inputFilePath);
            return FALSE;
        }
        
        *outputFile=fopen(outputFilePath, "w");
        if (!*outputFile) {
            printf("Could not open for writing: %s\n\n", outputFilePath);
            return FALSE;
        }
        
        fprintf(*inputFile, "CoinSpark %s Tests Input\n\n", testName);
        fprintf(*outputFile, "CoinSpark %s Tests Output\n\n", testName);
        printf(" Inputs: %s\nOutputs: %s\n", inputFilePath, outputFilePath);
    
    } else {
        *inputFile=NULL;
        *outputFile=NULL;
    }
    
    return TRUE;
}

void FlushTestFiles(FILE* inputFile, FILE* outputFile)
{
    if (inputFile)
        fflush(inputFile);

    if (outputFile)
        fflush(outputFile);
}

void FinishTests(FILE* inputFile, FILE* outputFile, const char* testName)
{
    printf("%s tests completed successfully.\n\n", testName);

    if (inputFile)
        fclose(inputFile);
    
    if (outputFile)
        fclose(outputFile);
}

bool PerformAddressTests(char* directoryName, int countTests, bool toDisplay)
{
    char testName[]="Address";
    FILE *inputFile, *outputFile;
    CoinSparkAddress encodeAddress, decodedAddress;
    char debugString[1024], encodeString[256];
    size_t encodeLen;

    if (!StartTests(&inputFile, &outputFile, directoryName, testName, countTests))
        return FALSE;
    
    while (countTests-->0) {
        RandomizeAddress(&encodeAddress);        
        
        CoinSparkAddressToString(&encodeAddress, debugString, sizeof(debugString));
        if (toDisplay)
            fputs(debugString, stdout);
        
        encodeLen=CoinSparkAddressEncode(&encodeAddress, encodeString, sizeof(encodeString));

        if (!encodeLen) {
            printf("Could not encode address!\n\n");
            return FALSE;
        }
        
        if (encodeLen!=strlen(encodeString)) {
            printf("Encoded length mismatch!\n\n");
            return FALSE;
        }
        
        if (toDisplay)
            printf("Encoded address: %s\n\n", encodeString);
        
        if (!CoinSparkAddressDecode(&decodedAddress, encodeString, encodeLen)) {
            printf("Could not decode address!\n\n");
            return FALSE;
        }
        
        if (!CoinSparkAddressMatch(&encodeAddress, &decodedAddress)) {
            printf("Decoded address does not match:\n\n");

            CoinSparkAddressToString(&decodedAddress, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            
            return FALSE;
        }

        if (toDisplay)
            printf("Decoded address matches\n\n");
        
        if (inputFile)
            fprintf(inputFile, "%s\n", encodeString);
        
        if (outputFile)
            fputs(debugString, outputFile);
        
        FlushTestFiles(inputFile, outputFile);

    //  Disturbance test for matching
        
        switch (rand()%3) {
            case 0:
                DisturbString(decodedAddress.bitcoinAddress);
                break;
                
            case 1:
                DisturbInteger32(&decodedAddress.addressFlags);
                break;
                
            case 2:
                DisturbInteger64(&decodedAddress.paymentRef);
                break;
        }
        
        if (CoinSparkAddressMatch(&encodeAddress, &decodedAddress)) {
            printf("Disturbed address should not match!\n\n");
            
            CoinSparkAddressToString(&encodeAddress, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            CoinSparkAddressToString(&decodedAddress, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            
            return FALSE;
        }
    }
    
    FinishTests(inputFile, outputFile, testName);
    
    return TRUE;
}

bool PerformAssetRefTests(char* directoryName, int countTests, bool toDisplay)
{
    char testName[]="AssetRef";
    FILE *inputFile, *outputFile;
    CoinSparkAssetRef encodeAssetRef, decodedAssetRef;
    char debugString[1024], encodeString[256];
    size_t encodeLen;
    
    if (!StartTests(&inputFile, &outputFile, directoryName, testName, countTests))
        return FALSE;
    
    while (countTests-->0) {
        RandomizeAssetRef(&encodeAssetRef);
        
        CoinSparkAssetRefToString(&encodeAssetRef, debugString, sizeof(debugString));
        if (toDisplay)
            fputs(debugString, stdout);
        
        encodeLen=CoinSparkAssetRefEncode(&encodeAssetRef, encodeString, sizeof(encodeString));
        
        if (!encodeLen) {
            printf("Could not encode asset reference!\n\n");
            return FALSE;
        }
        
        if (encodeLen!=strlen(encodeString)) {
            printf("Encoded length mismatch!\n\n");
            return FALSE;
        }
        
        if (toDisplay)
            printf("Encoded asset reference: %s\n\n", encodeString);
        
        if (!CoinSparkAssetRefDecode(&decodedAssetRef, encodeString, encodeLen)) {
            printf("Could not decode asset reference!\n\n");
            return FALSE;
        }
        
        if (!CoinSparkAssetRefMatch(&encodeAssetRef, &decodedAssetRef)) {
            printf("Decoded asset reference does not match:\n\n");
            
            CoinSparkAssetRefToString(&decodedAssetRef, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            
            return FALSE;
        }
        
        if (toDisplay)
            printf("Decoded asset reference matches\n\n");
        
        if (inputFile)
            fprintf(inputFile, "%s\n", encodeString);
        
        if (outputFile)
            fputs(debugString, outputFile);
        
        FlushTestFiles(inputFile, outputFile);

    //  Disturbance test for matching
        
        switch (rand()%3) {
            case 0:
                DisturbInteger64(&decodedAssetRef.blockNum);
                break;
                
            case 1:
                DisturbInteger64(&decodedAssetRef.txOffset);
                break;
                
            case 2:
                DisturbRawData(&decodedAssetRef.txIDPrefix, sizeof(decodedAssetRef.txIDPrefix));
                break;
        }
        
        if (CoinSparkAssetRefMatch(&encodeAssetRef, &decodedAssetRef)) {
            printf("Disturbed asset reference should not match!\n\n");
            
            CoinSparkAssetRefToString(&encodeAssetRef, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            CoinSparkAssetRefToString(&decodedAssetRef, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            
            return FALSE;
        }
    }
    
    FinishTests(inputFile, outputFile, testName);
    
    return TRUE;
}

bool PerformScriptTests(char* directoryName, int countTests, bool toDisplay)
{
    typedef enum {
        partGenesis=1,
        partPaymentRef=2,
        partTransfers=4,
        partMessage=8
    } MetadataTestType;
    
//  Encoding order when combined: genesis (always on its own), paymentRef, transfers, message
    
    MetadataTestType metadataTest[]={ // list of possible combinations to test
        partGenesis,
        partPaymentRef,
        partPaymentRef | partTransfers,
    //  partPaymentRef | partTransfers | partMessage, // doable but only with rare values
        partPaymentRef | partMessage,
        partTransfers,
        partTransfers | partMessage,
        partMessage,
    };
    
    char testName[]="Script";
    FILE *inputFile, *outputFile;
    MetadataTestType testType;
    CoinSparkGenesis encodeGenesis, decodedGenesis;
    CoinSparkPaymentRef encodePaymentRef, decodedPaymentRef;
    CoinSparkTransfer encodeTransfers[MAX_TRANSFERS], decodedTransfers[MAX_TRANSFERS], *disturbTransfer;
    CoinSparkMessage encodeMessage, decodedMessage;
    char debugString[16384], metadata[MAX_OP_RETURN_LEN], decodeMetadata[MAX_OP_RETURN_LEN], scriptPubKey[2*MAX_OP_RETURN_LEN+16], appendMetadata[MAX_OP_RETURN_LEN];
    int rounding, maxTransfers, transferIndex, countInputs, countOutputs, countEncodeTransfers, countDecodedTransfers, attempts;
    size_t metadataLen, scriptPubKeyLen, metadataMaxLen, appendMetadataLen;
    bool mustFillMetadata;
    
    if (!StartTests(&inputFile, &outputFile, directoryName, testName, countTests))
        return FALSE;
    
    while (countTests-->0) {
        testType=metadataTest[rand()%(sizeof(metadataTest)/sizeof(*metadataTest))];
        attempts=0;
        
    tryThisTestAgain:
        attempts++;
        metadataLen=0;
        mustFillMetadata=FALSE;
    
    //  Encoding...
        
        countInputs=1+rand()%(COINSPARK_IO_INDEX_MAX>>(rand()%14));
        countOutputs=1+rand()%(COINSPARK_IO_INDEX_MAX>>(rand()%14));
        
        if (testType&partGenesis) {
            RandomizeGenesis(&encodeGenesis);

            metadataLen=CoinSparkGenesisEncode(&encodeGenesis, metadata, sizeof(metadata));
            
            if (encodeGenesis.assetHashLen<COINSPARK_GENESIS_HASH_MAX_LEN)
                mustFillMetadata=TRUE;
        }
        
        if (testType&partPaymentRef) {
            encodePaymentRef=CoinSparkPaymentRefRandom();
            encodePaymentRef>>=(rand()%48);

            metadataLen=CoinSparkPaymentRefEncode(encodePaymentRef, metadata, sizeof(metadata));
        }
        
        if (testType&partTransfers) {
            if (metadataLen>0)
                metadataMaxLen=CoinSparkMetadataMaxAppendLen(metadata, metadataLen, sizeof(metadata));
            else
                metadataMaxLen=sizeof(metadata);
            
            maxTransfers=(testType&partMessage) ? 1 : MAX_TRANSFERS;
            
            for (transferIndex=0; transferIndex<maxTransfers; transferIndex++)
                RandomizeTransfer(encodeTransfers+transferIndex, (transferIndex>0) ? (encodeTransfers+rand()%transferIndex) : NULL,
                                  (maxTransfers==1) ? 0 : 10, countInputs, countOutputs);
            
            for (countEncodeTransfers=maxTransfers; countEncodeTransfers>0; countEncodeTransfers--) {
                appendMetadataLen=CoinSparkTransfersEncode(encodeTransfers, countEncodeTransfers, countInputs, countOutputs,
                    (metadataLen>0) ? appendMetadata : metadata, metadataMaxLen);
                
                if (appendMetadataLen>0) // we have enough space so stop reducing
                    break;
            }
            
            if (metadataLen>0)
                metadataLen=CoinSparkMetadataAppend(metadata, metadataLen, sizeof(metadata), appendMetadata, appendMetadataLen);
            else
                metadataLen=appendMetadataLen;
        }
        
        if (testType&partMessage) {
            if (metadataLen>0)
                metadataMaxLen=CoinSparkMetadataMaxAppendLen(metadata, metadataLen, sizeof(metadata));
            else
                metadataMaxLen=sizeof(metadata);
            
            if (RandomizeMessage(&encodeMessage, countOutputs, metadataMaxLen)) {
                appendMetadataLen=CoinSparkMessageEncode(&encodeMessage, countOutputs, (metadataLen>0) ? appendMetadata : metadata, metadataMaxLen);
                if (!appendMetadataLen) {
                    printf("Failed to encode message!\n\n");
                    return FALSE;
                }
                
                if (metadataLen>0)
                    metadataLen=CoinSparkMetadataAppend(metadata, metadataLen, sizeof(metadata), appendMetadata, appendMetadataLen);
                else
                    metadataLen=appendMetadataLen;
                
                if (encodeMessage.hashLen<COINSPARK_MESSAGE_HASH_MAX_LEN)
                    mustFillMetadata=TRUE;

            } else
                goto tryThisTestAgain; // we don't have space for any message so have another try
        }
    
    //  Display encoded data and outputting it to file
        
        if (toDisplay || outputFile) {
            if (toDisplay)
                printf("After %d attempt%s...\n\n", attempts, (attempts==1) ? "" : "s");

            if (testType&partGenesis) {
                CoinSparkGenesisToString(&encodeGenesis, debugString, sizeof(debugString));
                if (toDisplay)
                    fputs(debugString, stdout);
                if (outputFile)
                    fputs(debugString, outputFile);
            }
            
            if (testType&partPaymentRef) {
                CoinSparkPaymentRefToString(encodePaymentRef, debugString, sizeof(debugString));
                if (toDisplay)
                    fputs(debugString, stdout);
                if (outputFile)
                    fputs(debugString, outputFile);
            }
            
            if (testType&partTransfers) {
                CoinSparkTransfersToString(encodeTransfers, countEncodeTransfers, debugString, sizeof(debugString));
                if (toDisplay)
                    fputs(debugString, stdout);
                if (outputFile)
                    fputs(debugString, outputFile);
            }
            
            if (testType&partMessage) {
                CoinSparkMessageToString(&encodeMessage, debugString, sizeof(debugString));
                if (toDisplay)
                    fputs(debugString, stdout);
                if (outputFile)
                    fputs(debugString, outputFile);
            }
        }

    //  Displaying raw data and global checks
        
        if (!metadataLen) {
            printf("Failed to encode metadata!\n\n");
            return FALSE;
        }

        if (toDisplay)
            DisplayRawData(metadata, metadataLen, stdout);

        if (mustFillMetadata && (metadataLen!=sizeof(metadata))) {
            printf("Failed to use all available metadata!\n\n");
            return FALSE;
        }
        
    //  Raw binary scripts
        
        scriptPubKeyLen=CoinSparkMetadataToScript(metadata, metadataLen, scriptPubKey, sizeof(scriptPubKey), FALSE);
        
        if (!scriptPubKeyLen) {
            printf("Failed to convert metadata to raw script!\n\n");
            return FALSE;
        }
        
        if (CoinSparkScriptToMetadata(scriptPubKey, scriptPubKeyLen, FALSE, decodeMetadata, sizeof(decodeMetadata))!=metadataLen) {
            printf("Raw script back to metadata unexpected length!\n\n");
            return FALSE;
        }
        
        if (memcmp(metadata, decodeMetadata, metadataLen)) {
            DisplayRawData(decodeMetadata, metadataLen, stdout);
            
            printf("Raw script back to metadata mismatch!\n\n");
            return FALSE;
        }
        
    //  Hexadecimal scripts (and this one is output)
        
        scriptPubKeyLen=CoinSparkMetadataToScript(metadata, metadataLen, scriptPubKey, sizeof(scriptPubKey), TRUE);
        
        if (!scriptPubKeyLen) {
            printf("Failed to convert metadata to hex script!\n\n");
            return FALSE;
        }
       
        if (CoinSparkScriptToMetadata(scriptPubKey, scriptPubKeyLen, TRUE, decodeMetadata, sizeof(decodeMetadata))!=metadataLen) {
            printf("Hex script back to metadata unexpected length!\n\n");
            return FALSE;
        }
        
        if (memcmp(metadata, decodeMetadata, metadataLen)) {
            DisplayRawData(decodeMetadata, metadataLen, stdout);
            
            printf("Hex script back to metadata mismatch!\n\n");
            return FALSE;
        }
        
        if (inputFile) {
            fprintf(inputFile, "%d # number of tx inputs\n%d # number of tx outputs\n", countInputs, countOutputs);
            fprintf(inputFile, "%s # metadata script\n\n", scriptPubKey);
        }
        
    //  Decoding...
        
        if (testType&partGenesis) {
            if (!CoinSparkGenesisDecode(&decodedGenesis, metadata, metadataLen)) {
                printf("Failed to decode genesis!\n\n");
                return FALSE;
            }
            
            if (!CoinSparkGenesisMatch(&encodeGenesis, &decodedGenesis, TRUE)) {
                printf("Decoded genesis does not match:\n\n");
                
                CoinSparkGenesisToString(&decodedGenesis, debugString, sizeof(debugString));
                fputs(debugString, stdout);
               
                return FALSE;
            }
            
            rounding=(rand()%3)-1;
            
            CoinSparkGenesisSetQty(&decodedGenesis, 0, 0);
            CoinSparkGenesisSetQty(&decodedGenesis, CoinSparkGenesisGetQty(&encodeGenesis), rounding);
            
            CoinSparkGenesisSetChargeFlat(&decodedGenesis, 0, 0);
            CoinSparkGenesisSetChargeFlat(&decodedGenesis, CoinSparkGenesisGetChargeFlat(&encodeGenesis), rounding);
 
            if (!CoinSparkGenesisMatch(&encodeGenesis, &decodedGenesis, FALSE)) {
                printf("Setting genesis quantities failed:\n\n");
                
                CoinSparkGenesisToString(&decodedGenesis, debugString, sizeof(debugString));
                fputs(debugString, stdout);
                
                return FALSE;
            
            } else if (toDisplay)
                printf("Successful match of genesis.\n");
         }

        if (testType&partPaymentRef) {
            if (!CoinSparkPaymentRefDecode(&decodedPaymentRef, metadata, metadataLen)) {
                printf("Failed to decode payment reference!\n\n");
                return FALSE;
            }
            
            if (encodePaymentRef != decodedPaymentRef) {
                printf("Decoded payment reference does not match:\n\n");

                CoinSparkPaymentRefToString(decodedPaymentRef, debugString, sizeof(debugString));
                fputs(debugString, stdout);

                return FALSE;
           
            } else if (toDisplay)
                printf("Successful match of payment reference.\n");
        }
        
        if (testType&partTransfers) {
            countDecodedTransfers=CoinSparkTransfersDecodeCount(metadata, metadataLen);
            
            if (countDecodedTransfers!=countEncodeTransfers) {
                printf("Decode transfer count not as expected!\n\n");
                return FALSE;
            }
            
            countDecodedTransfers=CoinSparkTransfersDecode(decodedTransfers, MAX_TRANSFERS, countInputs, countOutputs,
                                                           metadata, metadataLen);
            
            if (countDecodedTransfers!=countEncodeTransfers) {
                printf("Number of decoded transfers not as expected!\n\n");
                return FALSE;
            }
            
            if (!CoinSparkTransfersMatch(encodeTransfers, encodeTransfers, countEncodeTransfers, TRUE)) {
                printf("Encoded transfers do not match themselves!\n\n");
                return FALSE;
            }
            
            if (!CoinSparkTransfersMatch(encodeTransfers, decodedTransfers, countEncodeTransfers, FALSE)) {
                printf("Decoded transfers do not match:\n\n");
 
                CoinSparkTransfersToString(decodedTransfers, countDecodedTransfers, debugString, sizeof(debugString));
                fputs(debugString, stdout);
               
                return FALSE;

            } else if (toDisplay)
                printf("Successful match of transfers.\n");
        }
        
        if (testType&partMessage) {
            if (!CoinSparkMessageDecode(&decodedMessage, countOutputs, metadata, metadataLen)) {
                printf("Failed to decode message!\n\n");
                return FALSE;
            }
        
            if (!CoinSparkMessageMatch(&encodeMessage, &encodeMessage, TRUE)) {
                printf("Encoded message does not itself:\n\n");
                return FALSE;
            }

            if (!CoinSparkMessageMatch(&encodeMessage, &decodedMessage, FALSE)) {
                printf("Decoded message does not match:\n\n");
                
                CoinSparkMessageToString(&decodedMessage, debugString, sizeof(debugString));
                fputs(debugString, stdout);
               
                return FALSE;

            } else if (toDisplay)
                printf("Successful match of message.\n");
        }

        if (toDisplay)
            printf("All metadata decoding passed.\n\n");
        
        FlushTestFiles(inputFile, outputFile);
        
    //  Disturbance tests for matching
        
        if (testType&partGenesis) {
            decodedGenesis=encodeGenesis;
            
            disturbGenesisOther:
            switch (rand()%8) {
                case 0:
                    DisturbInteger16(&decodedGenesis.qtyMantissa);
                    break;
                    
                case 1:
                    DisturbInteger16(&decodedGenesis.qtyExponent);
                    break;
                    
                case 2:
                    DisturbInteger16(&decodedGenesis.chargeFlatMantissa);
                    break;
                    
                case 3:
                    if (decodedGenesis.chargeFlatMantissa==0)
                        goto disturbGenesisOther;
                    
                    DisturbInteger16(&decodedGenesis.chargeFlatExponent);
                    break;
                    
                case 4:
                    DisturbInteger16(&decodedGenesis.chargeBasisPoints);
                    break;
                    
                case 5:
                    decodedGenesis.useHttps=!decodedGenesis.useHttps;
                    break;
                    
                case 6:
                    DisturbString(decodedGenesis.domainName);
                    break;
                    
                case 7:
                    DisturbRawData(decodedGenesis.assetHash, decodedGenesis.assetHashLen);
                    break;
            }
            
            if (
                CoinSparkGenesisMatch(&encodeGenesis, &decodedGenesis, TRUE) ||
                (CoinSparkGenesisIsValid(&decodedGenesis) && CoinSparkGenesisMatch(&encodeGenesis, &decodedGenesis, FALSE))
            ) {
                printf("Disturbed genesis should not match!\n\n");
                
                CoinSparkGenesisToString(&encodeGenesis, debugString, sizeof(debugString));
                fputs(debugString, stdout);
                CoinSparkGenesisToString(&decodedGenesis, debugString, sizeof(debugString));
                fputs(debugString, stdout);
                
                return FALSE;
            }
        }
        
        if (testType&partTransfers) {
            disturbTransferOther:

            disturbTransfer=decodedTransfers+rand()%countDecodedTransfers;
            
            switch (rand()%8) {
                case 0:
                    DisturbInteger64(&disturbTransfer->assetRef.blockNum);
                    break;

                case 1:
                    if (disturbTransfer->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
                        goto disturbTransferOther;
                    
                    DisturbInteger64(&disturbTransfer->assetRef.txOffset);
                    break;
                    
                case 2:
                    if (disturbTransfer->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
                        goto disturbTransferOther;
                    
                    DisturbRawData(&disturbTransfer->assetRef.txIDPrefix, sizeof(disturbTransfer->assetRef.txIDPrefix));
                    break;
                    
                case 3:
                    DisturbInteger32(&disturbTransfer->inputs.first);
                    break;

                case 4:
                    DisturbInteger32(&disturbTransfer->inputs.count);
                    break;
                    
                case 5:
                    DisturbInteger32(&disturbTransfer->outputs.first);
                    break;
                    
                case 6:
                    if (disturbTransfer->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
                        goto disturbTransferOther;
                    
                    DisturbInteger32(&disturbTransfer->outputs.count);
                    break;
                    
                case 7:
                    if (disturbTransfer->assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
                        goto disturbTransferOther;
                    
                    DisturbInteger64(&disturbTransfer->qtyPerOutput);
                    break;
            }
            
            if (
                CoinSparkTransfersMatch(encodeTransfers, decodedTransfers, countEncodeTransfers, TRUE) ||
                CoinSparkTransfersMatch(encodeTransfers, decodedTransfers, countEncodeTransfers, FALSE)
            ) {
                printf("Disturbed transfers should not match!\n\n");
                
                CoinSparkTransfersToString(encodeTransfers, countEncodeTransfers, debugString, sizeof(debugString));
                fputs(debugString, stdout);
                CoinSparkTransfersToString(decodedTransfers, countDecodedTransfers, debugString, sizeof(debugString));
                fputs(debugString, stdout);
                
                return FALSE;
            }
            
            if (testType&partMessage) {
                disturbMessageOther:
                
                decodedMessage=encodeMessage;
                
                switch (rand()%8) {
                    case 0:
                        decodedMessage.useHttps=!decodedMessage.useHttps;
                        break;
                        
                    case 1:
                        DisturbString(decodedMessage.serverHost);
                        break;
                        
                    case 2:
                        decodedMessage.usePrefix=!decodedMessage.usePrefix;
                        break;
                        
                    case 3:
                        if (decodedMessage.serverPath[0])
                            DisturbString(decodedMessage.serverPath);
                        else
                            goto disturbMessageOther;
                        break;
                        
                    case 4:
                        decodedMessage.isPublic=!decodedMessage.isPublic;
                        break;
                        
                    case 5:
                        if (decodedMessage.countOutputRanges>0)
                            decodedMessage.countOutputRanges--;
                        else
                            goto disturbMessageOther;
                        break;
                        
                    case 6:
                        if (decodedMessage.countOutputRanges>0)
                            DisturbIORange(decodedMessage.outputRanges+rand()%decodedMessage.countOutputRanges);
                        else
                            goto disturbMessageOther;
                        break;
                        
                    case 7:
                        DisturbRawData(decodedMessage.hash, decodedMessage.hashLen);
                        break;
                }
                
                if (CoinSparkMessageMatch(&encodeMessage, &decodedMessage, TRUE)) {
                    printf("Disturbed message should not match!\n\n");
                    
                    CoinSparkMessageToString(&encodeMessage, debugString, sizeof(debugString));
                    fputs(debugString, stdout);
                    CoinSparkMessageToString(&decodedMessage, debugString, sizeof(debugString));
                    fputs(debugString, stdout);
                
                //    return FALSE;
                }
            }
        }
        
    }

    FinishTests(inputFile, outputFile, testName);
    
    return TRUE;
}

bool PerformAssetHashTests(char* directoryName, int countTests, bool toDisplay)
{
    char testName[]="AssetHash";
    FILE *inputFile, *outputFile;
    char name[32], issuer[32], description[256], units[8], issueDate[16], expiryDate[16], contractContent[1024];
    double interestRate, multiple;
    unsigned char assetHash[32];
    
    if (!StartTests(&inputFile, &outputFile, directoryName, testName, countTests))
        return FALSE;
    
    while (countTests-->0) {
        RandomizeReadableString(name, sizeof(name), TRUE);
        RandomizeReadableString(issuer, sizeof(issuer), TRUE);
        RandomizeReadableString(description, sizeof(description), TRUE);
        RandomizeReadableString(units, sizeof(units), TRUE);
        RandomizeReadableString(issueDate, sizeof(issueDate), TRUE);
        RandomizeReadableString(expiryDate, sizeof(expiryDate), TRUE);
        RandomizeReadableString(contractContent, sizeof(contractContent), FALSE);
        
        interestRate=6*((double)rand())/RAND_MAX;
        multiple=pow(10, rand()%13-6);
        
        CoinSparkCalcAssetHash(name, strlen(name), issuer, strlen(issuer), description, strlen(description), units, strlen(units),
            issueDate, strlen(issueDate), expiryDate, strlen(expiryDate), &interestRate, &multiple, contractContent, strlen(contractContent), assetHash);
        
        // we use 6 decimal places of precision here because this is the same as that used for the asset hash calculation
        // otherwise there can be cases where we output a number exactly 0.5 way between two values and the hashes are different
        
        if (inputFile)
            fprintf(inputFile, "%s # name\n%s # issuer\n%s # description\n%s # units\n%s # issue date\n%s # expiry date\n%0.6f # interest rate\n%0.6f # multiple\n%s # contract\n\n", name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent);
        
        if (outputFile) {
            DisplayHexadecimal(assetHash, sizeof(assetHash), outputFile);
            fputc('\n', outputFile);
        }
        
        FlushTestFiles(inputFile, outputFile);
    }

    FinishTests(inputFile, outputFile, testName);
    
    return TRUE;
}

bool PerformGenesisTests(char* directoryName, int countTests, bool toDisplay)
{
    char testName[]="Genesis";
    FILE *inputFile, *outputFile;
    int countOutputs, outputIndex, charIndex, firstSpentVout;
    bool outputsRegular[MAX_OUTPUTS], firstIrregular;
    char metadata[MAX_OP_RETURN_LEN], debugString[1024], scriptPubKeysData[MAX_OUTPUTS][MAX_OP_RETURN_LEN+2], urlString[256], firstSpentTxIdString[65], *testUrlString;
    const char *scriptPubKeysPtrs[MAX_OUTPUTS];
    size_t metadataLen, scriptPubKeysLen[MAX_OUTPUTS];
    u_int8_t firstSpentTxId[32];
    CoinSparkGenesis genesis;
    CoinSparkSatoshiQty outputsSatoshis[MAX_OUTPUTS], validFeeSatoshis, feeSatoshis;
    CoinSparkAssetQty outputBalances[MAX_OUTPUTS], outputBalancesTotal, testOutputBalance;
    
    if (!StartTests(&inputFile, &outputFile, directoryName, testName, countTests))
        return FALSE;

    while (countTests-->0) {
        
    //  Randomize the genesis transaction
        
        countOutputs=1+rand()%MAX_OUTPUTS;
        
        RandomizeGenesis(&genesis);

        metadataLen=CoinSparkGenesisEncode(&genesis, metadata, sizeof(metadata));
        if (!metadataLen) {
            printf("Could not encoded genesis as metadata!\n\n");
            return FALSE;
        }
        
        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            outputsSatoshis[outputIndex]=rand()%100000;
            outputsRegular[outputIndex]=(rand()%10) ? TRUE : FALSE; // 90% of outputs are regular
        }
        
        outputsRegular[rand()%countOutputs]=FALSE; // ensure at least one output is irregular
        
        RandomizeRawData(firstSpentTxId, sizeof(firstSpentTxId));        
        for (charIndex=0; charIndex<sizeof(firstSpentTxId); charIndex++)
            sprintf(firstSpentTxIdString+2*charIndex, "%02x", firstSpentTxId[charIndex]);
        firstSpentVout=rand()%256;
        
    //  The bitcoin transaction fee
        
        validFeeSatoshis=CoinSparkGenesisCalcMinFee(&genesis, outputsSatoshis, outputsRegular, countOutputs);
        
        if (rand()%3) { // probably sufficient
            feeSatoshis=validFeeSatoshis+rand()%100000;
            feeSatoshis-=COINSPARK_MIN(feeSatoshis, 10000); // min to prevent overflow
            
        } else
            feeSatoshis=rand()%100; // probably insufficient

    //  Display the full scenario
        
        if (toDisplay) {
            printf("First spent transaction ID: %s\n", firstSpentTxIdString);
            printf("First spent output index: %d\n\n", firstSpentVout);
            
            CoinSparkGenesisToString(&genesis, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                printf("Output %d satoshis: %lld (%s)\n", outputIndex, (long long)(outputsSatoshis[outputIndex]),
                    outputsRegular[outputIndex] ? "regular" : "irregular");
            
            printf("Fee satoshis: %lld (%s)\n\n", (long long)feeSatoshis, (feeSatoshis>=validFeeSatoshis) ? "valid" : "invalid");
        }
        
    //  Write transfer scenario to file
        
        if (inputFile) {
            fprintf(inputFile, "%s # first spent bitcoin txid\n", firstSpentTxIdString);
            fprintf(inputFile, "%d # first spent output index\n", firstSpentVout);
            
            DisplayHexadecimal(metadata, metadataLen, inputFile);
            fprintf(inputFile, " # genesis encoded as metadata\n");
            
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                fprintf(inputFile, "%s%lld", outputIndex ? "," : "", (long long)(outputsSatoshis[outputIndex]));
            fprintf(inputFile, " # bitcoin satoshis in each output\n");
                    
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                fprintf(inputFile, "%s%d", outputIndex ? "," : "", outputsRegular[outputIndex]);
            fprintf(inputFile, " # boolean flags whether each bitcoin output is 'regular'\n");
            
            fprintf(inputFile, "%lld # bitcoin satoshis in the transaction fee\n\n", (long long)feeSatoshis);
        }
        
    //  Perform the genesis calculation and calculate URL
        
        if (feeSatoshis>=validFeeSatoshis)
            CoinSparkGenesisApply(&genesis, outputsRegular, outputBalances, countOutputs);
        else
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                outputBalances[outputIndex]=0;
        
        CoinSparkGenesisCalcAssetURL(&genesis, firstSpentTxIdString, firstSpentVout, urlString, sizeof(urlString));
        
    //  Display the outcome
        
        if (toDisplay) {
            outputBalancesTotal=0;
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
                printf("Output %d quantity: %lld\n", outputIndex, (long long)(outputBalances[outputIndex]));
                outputBalancesTotal+=outputBalances[outputIndex];
            }
            
            printf("Output total quantity: %lld\n\n", (long long)outputBalancesTotal);

            printf("Asset web page URL: %s\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n", urlString);
        }
        
    //  Writing outcome to file
        
        if (outputFile) {
            fprintf(outputFile, "%lld # transaction fee satoshis to be valid\n", (long long)validFeeSatoshis);
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                fprintf(outputFile, "%s%lld", outputIndex ? "," : "", (long long)(outputBalances[outputIndex]));
            fprintf(outputFile, " # units of the asset in each output\n");
            fprintf(outputFile, "%s # asset web page URL\n\n", urlString);
        }
        
    //  Test the functions used in the contract template
        
        firstIrregular=TRUE;
        
        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            if (firstIrregular && !outputsRegular[outputIndex]) {
                scriptPubKeysLen[outputIndex]=CoinSparkMetadataToScript(metadata, metadataLen,
                    scriptPubKeysData[outputIndex], sizeof(scriptPubKeysData[outputIndex]), FALSE);
                firstIrregular=FALSE;
            
            } else {
                scriptPubKeysLen[outputIndex]=RandomizeRegularScript(scriptPubKeysData[outputIndex],
                                                                     sizeof(scriptPubKeysData[outputIndex]));
                if (!outputsRegular[outputIndex])
                    scriptPubKeysData[outputIndex][0]=0x6a; // make it look like an OP_RETURN
            }
            
            scriptPubKeysPtrs[outputIndex]=scriptPubKeysData[outputIndex];
        }
        
        testUrlString=CoinSparkGetGenesisWebPageURL(scriptPubKeysPtrs, scriptPubKeysLen, countOutputs, firstSpentTxId, firstSpentVout);
        if (strcmp(urlString, testUrlString)) {
            printf("CoinSparkGetGenesisWebPageURL() mismatch: %s should be %s!\n", testUrlString, urlString);
            return FALSE;
        }
        free(testUrlString);
        
        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            testOutputBalance=CoinSparkGetGenesisOutputQty(scriptPubKeysPtrs, scriptPubKeysLen, outputsSatoshis, countOutputs,
                                                           feeSatoshis, outputIndex);
            
            if (testOutputBalance!=outputBalances[outputIndex]) {
                printf("CoinSparkGetGenesisOutputQty() mismatch for output %d: %lld should be %lld!\n",
                       outputIndex, (long long)testOutputBalance, (long long)(outputBalances[outputIndex]));
                return FALSE;
            }
        }

    //  Finish the loop
        
        FlushTestFiles(inputFile, outputFile);
    }
    
    FinishTests(inputFile, outputFile, testName);
    
    return TRUE;
}

bool PerformTransferTests(char* directoryName, int countTests, bool toDisplay)
{
    char testName[]="Transfer";
    FILE *inputFile, *outputFile;
    int countInputs, countOutputs, inputIndex, transferIndex, countEncodeTransfers, outputIndex, genesisOutputs, opReturnOutputIndex;
    size_t transfersMetadataLen, genesisMetadataLen, genesisScriptPubKeysLen[MAX_OUTPUTS], thisScriptPubKeysLen[MAX_OUTPUTS];
    char transfersMetadata[MAX_OP_RETURN_LEN], genesisMetadata[MAX_OP_RETURN_LEN], debugString[1024*MAX_TRANSFERS];
    char genesisScriptPubKeysData[MAX_OUTPUTS][MAX_OP_RETURN_LEN+2], thisScriptPubKeysData[MAX_OUTPUTS][MAX_OP_RETURN_LEN+2];
    u_int8_t genesisTxId[32];
    const char *genesisScriptPubKeysPtrs[MAX_OUTPUTS], *thisScriptPubKeysPtrs[MAX_OUTPUTS];
    bool outputsRegular[MAX_OUTPUTS], genesisOutputsRegular[MAX_OUTPUTS], firstIrregular, outputsDefault[MAX_OUTPUTS];
    CoinSparkAssetRef assetRef;
    CoinSparkAssetQty inputBalances[MAX_INPUTS], inputBalancesTotal, outputBalances[MAX_OUTPUTS], outputBalancesTotal;
    CoinSparkAssetQty testOutputBalance, testNetBalance, testGrossBalance;
    CoinSparkSatoshiQty outputsSatoshis[MAX_OUTPUTS], validFeeSatoshis, feeSatoshis, genesisOutputsSatoshis[MAX_OUTPUTS], genesisFeeSatoshis;
    CoinSparkTransfer encodeTransfers[MAX_TRANSFERS];
    CoinSparkGenesis genesis;
    
    if (!StartTests(&inputFile, &outputFile, directoryName, testName, countTests))
        return FALSE;
    
    while (countTests-->0) {
        countInputs=1+rand()%MAX_INPUTS;
        countOutputs=1+rand()%MAX_OUTPUTS;
        
    //  Asset whose transfer we will be simulating

        RandomizeGenesis(&genesis);
        RandomizeAssetRef(&assetRef);
        
        genesisMetadataLen=CoinSparkGenesisEncode(&genesis, genesisMetadata, sizeof(genesisMetadata));
        if (!genesisMetadataLen) {
            printf("Could not encoded genesis as metadata!\n\n");
            return FALSE;
        }
        
    //  The list of transfers to be encoded
        
        for (transferIndex=0; transferIndex<MAX_TRANSFERS; transferIndex++) {
            RandomizeTransfer(encodeTransfers+transferIndex, (transferIndex>0) ? (encodeTransfers+rand()%transferIndex) : NULL,
                              20, countInputs, countOutputs);
            if (rand()%5)
                encodeTransfers[transferIndex].assetRef=assetRef; // 80% of them will be related to this asset
        }
        
        for (countEncodeTransfers=MAX_TRANSFERS; countEncodeTransfers>0; countEncodeTransfers--) {
            transfersMetadataLen=CoinSparkTransfersEncode(encodeTransfers, countEncodeTransfers, countInputs, countOutputs,
                transfersMetadata, sizeof(transfersMetadata));
            
            if (transfersMetadataLen>0) // this number can be fit in
                break;
        }
        
    //  Asset quantities in transaction inputs
        
        for (inputIndex=0; inputIndex<countInputs; inputIndex++) { // input quantities
            inputBalances[inputIndex]=RandomizeAssetQty(NULL);
            inputBalances[inputIndex]=COINSPARK_MIN(inputBalances[inputIndex], COINSPARK_ASSET_QTY_MAX/MAX_OUTPUTS);
        }

    //  The quantity of bitcoin in each output, and whether the output is regular
        
        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            outputsSatoshis[outputIndex]=rand()%100000;
            outputsRegular[outputIndex]=(rand()%10) ? TRUE : FALSE; // 90% of outputs are regular
        }
        
        outputsRegular[rand()%countOutputs]=FALSE; // ensure at least one output is irregular
        
    //  The bitcoin transaction fee
        
        validFeeSatoshis=CoinSparkTransfersCalcMinFee(encodeTransfers, countEncodeTransfers, countInputs, countOutputs,
            outputsSatoshis, outputsRegular);
        
        if (rand()%3) { // probably sufficient
            feeSatoshis=validFeeSatoshis+rand()%100000;
            feeSatoshis-=COINSPARK_MIN(feeSatoshis, 10000); // min to prevent overflow
            
        } else
            feeSatoshis=rand()%100; // probably insufficient
        
    //  Display the full scenario
        
        if (toDisplay) {
            CoinSparkGenesisToString(&genesis, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            
            CoinSparkAssetRefToString(&assetRef, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            
            CoinSparkTransfersToString(encodeTransfers, countEncodeTransfers, debugString, sizeof(debugString));
            fputs(debugString, stdout);
            
            inputBalancesTotal=0;
            for (inputIndex=0; inputIndex<countInputs; inputIndex++) {
                printf("Input %d quantity: %lld\n", inputIndex, (long long)(inputBalances[inputIndex]));
                inputBalancesTotal+=inputBalances[inputIndex];
            }
            
            printf("Input total quantity: %lld\n\n", (long long)inputBalancesTotal);
            
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                printf("Output %d satoshis: %lld (%s)\n", outputIndex, (long long)(outputsSatoshis[outputIndex]),
                    outputsRegular[outputIndex] ? "regular" : "irregular");
            
            printf("Fee satoshis: %lld (%s)\n\n", (long long)feeSatoshis, (feeSatoshis>=validFeeSatoshis) ? "valid" : "invalid");
        }
        
    //  Write transfer scenario to file
        
        if (inputFile) {
            DisplayHexadecimal(genesisMetadata, genesisMetadataLen, inputFile);
            fprintf(inputFile, " # this asset genesis encoded as metadata\n");
            
            CoinSparkAssetRefEncode(&assetRef, debugString, sizeof(debugString));
            fprintf(inputFile, "%s # asset reference to this genesis\n", debugString);
            
            DisplayHexadecimal(transfersMetadata, transfersMetadataLen, inputFile);
            fprintf(inputFile, " # list of transfers encoded as metadata\n");
            
            for (inputIndex=0; inputIndex<countInputs; inputIndex++)
                fprintf(inputFile, "%s%lld", inputIndex ? "," : "", (long long)(inputBalances[inputIndex]));
            fprintf(inputFile, " # units of this asset in each input\n");
            
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                fprintf(inputFile, "%s%lld", outputIndex ? "," : "", (long long)(outputsSatoshis[outputIndex]));
            fprintf(inputFile, " # bitcoin satoshis in each output\n");
                    
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                fprintf(inputFile, "%s%d", outputIndex ? "," : "", outputsRegular[outputIndex]);
            fprintf(inputFile, " # boolean flags whether each bitcoin output is 'regular'\n");
            
            fprintf(inputFile, "%lld # bitcoin satoshis in the transaction fee\n\n", (long long)feeSatoshis);
        }
        
    //  Perform the transfer calculation and get default flags
        
        if (feeSatoshis>=validFeeSatoshis)
            CoinSparkTransfersApply(&assetRef, &genesis, encodeTransfers, countEncodeTransfers,
                                    inputBalances, countInputs, outputsRegular, outputBalances, countOutputs);
        else
            CoinSparkTransfersApplyNone(&assetRef, &genesis,
                                    inputBalances, countInputs, outputsRegular, outputBalances, countOutputs);
        
        CoinSparkTransfersDefaultOutputs(encodeTransfers, countEncodeTransfers, countInputs,
                                         outputsRegular, outputsDefault, countOutputs);
        
    //  Display the outcome
        
        if (toDisplay) {
            outputBalancesTotal=0;
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
                printf("Output %d quantity: %lld%s\n", outputIndex, (long long)(outputBalances[outputIndex]), outputsDefault[outputIndex] ? " [is default]" : "");
                outputBalancesTotal+=outputBalances[outputIndex];
            }
            
            printf("Output total quantity: %lld\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n", (long long)outputBalancesTotal);
        }
        
    //  Writing outcome to file
        
        if (outputFile) {
            fprintf(outputFile, "%lld # transaction fee satoshis to be valid\n", (long long)validFeeSatoshis);
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                fprintf(outputFile, "%s%lld", outputIndex ? "," : "", (long long)(outputBalances[outputIndex]));
            fprintf(outputFile, " # units of this asset in each output\n");
            for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
                fprintf(outputFile, "%s%d", outputIndex ? "," : "", outputsDefault[outputIndex] ? 1 : 0);
            fprintf(outputFile, " # boolean flags whether each output is in a default route\n\n");
        }
        
    //  Test the net and gross calculations using the input balances as example net values
        
        for (inputIndex=0; inputIndex<countInputs; inputIndex++) {
            testGrossBalance=CoinSparkGenesisCalcGross(&genesis, inputBalances[inputIndex]);
            testNetBalance=CoinSparkGenesisCalcNet(&genesis, testGrossBalance);
            
            if (inputBalances[inputIndex]!=testNetBalance) {
                printf("Net to gross to net mismatch: %lld -> %lld -> %lld!\n",
                       (long long)(inputBalances[inputIndex]), (long long)testGrossBalance, (long long)testNetBalance);
                return FALSE;
            }
        }
        
    //  Test the CoinSparkGetTransferOutputQty() function used in the contract template
        
        genesisOutputs=1+rand()%MAX_OUTPUTS;
        opReturnOutputIndex=rand()%genesisOutputs;

        for (outputIndex=0; outputIndex<genesisOutputs; outputIndex++) {
            if (outputIndex==opReturnOutputIndex) {
                genesisScriptPubKeysLen[outputIndex]=CoinSparkMetadataToScript(genesisMetadata, genesisMetadataLen,
                    genesisScriptPubKeysData[outputIndex], sizeof(genesisScriptPubKeysData[outputIndex]), FALSE);
                genesisOutputsSatoshis[outputIndex]=0;
                
            } else {
                genesisScriptPubKeysLen[outputIndex]=RandomizeRegularScript(genesisScriptPubKeysData[outputIndex],
                    sizeof(genesisScriptPubKeysData[outputIndex]));
                genesisOutputsSatoshis[outputIndex]=rand()%100000;
            }
            
            genesisOutputsRegular[outputIndex]=CoinSparkScriptIsRegular(genesisScriptPubKeysData[outputIndex], genesisScriptPubKeysLen[outputIndex], FALSE);
            genesisScriptPubKeysPtrs[outputIndex]=genesisScriptPubKeysData[outputIndex];
        }
        
        genesisFeeSatoshis=CoinSparkGenesisCalcMinFee(&genesis, genesisOutputsSatoshis, genesisOutputsRegular, genesisOutputs);
        
        memcpy(genesisTxId, assetRef.txIDPrefix, COINSPARK_ASSETREF_TXID_PREFIX_LEN);
        RandomizeRawData(genesisTxId+COINSPARK_ASSETREF_TXID_PREFIX_LEN, 32-COINSPARK_ASSETREF_TXID_PREFIX_LEN);
        
        firstIrregular=TRUE;
        
        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            if (firstIrregular && !outputsRegular[outputIndex]) {
                thisScriptPubKeysLen[outputIndex]=CoinSparkMetadataToScript(transfersMetadata, transfersMetadataLen,
                    thisScriptPubKeysData[outputIndex], sizeof(thisScriptPubKeysData[outputIndex]), FALSE);
                firstIrregular=FALSE;
            
            } else {
                thisScriptPubKeysLen[outputIndex]=RandomizeRegularScript(thisScriptPubKeysData[outputIndex],
                                                                         sizeof(thisScriptPubKeysData[outputIndex]));
                if (!outputsRegular[outputIndex])
                    thisScriptPubKeysData[outputIndex][0]=0x6a; // make it look like an OP_RETURN
            }
            
            thisScriptPubKeysPtrs[outputIndex]=thisScriptPubKeysData[outputIndex];
        }
        
        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            testOutputBalance=CoinSparkGetTransferOutputQty(genesisScriptPubKeysPtrs, genesisScriptPubKeysLen,
                                                            genesisOutputsSatoshis, genesisOutputs,
                                                            genesisFeeSatoshis,
                                                            assetRef.blockNum, assetRef.txOffset, genesisTxId,
                                                            inputBalances, countInputs,
                                                            thisScriptPubKeysPtrs, thisScriptPubKeysLen,
                                                            outputsSatoshis, countOutputs,
                                                            feeSatoshis, outputIndex);

            if (testOutputBalance!=outputBalances[outputIndex]) {
                printf("CoinSparkGetTransferOutputQty() mismatch for output %d: %lld should be %lld!\n",
                       outputIndex, (long long)testOutputBalance, (long long)(outputBalances[outputIndex]));
                return FALSE;
            }
        }
        
    //  Finish the loop
        
        FlushTestFiles(inputFile, outputFile);
    }
    
    FinishTests(inputFile, outputFile, testName);
    
    return TRUE;
}

bool PerformMessageHashTests(char* directoryName, int countTests, bool toDisplay)
{
    char testName[]="MessageHash";
    FILE *inputFile, *outputFile;
    char salt[32], mimeType[MAX_MESSAGE_PARTS][32], fileName[MAX_MESSAGE_PARTS][32], content[MAX_MESSAGE_PARTS][1024];
    CoinSparkMessagePart messageParts[MAX_MESSAGE_PARTS];
    int partIndex, countParts;
    unsigned char messageHash[32];
    
    if (!StartTests(&inputFile, &outputFile, directoryName, testName, countTests))
        return FALSE;
    
    while (countTests-->0) {
        countParts=1+rand()%MAX_MESSAGE_PARTS;
        RandomizeReadableString(salt, sizeof(salt), FALSE);
        
        for (partIndex=0; partIndex<countParts; partIndex++) {
            RandomizeReadableString(mimeType[partIndex], sizeof(mimeType[partIndex]), FALSE);

            if (rand()%2)
                RandomizeReadableString(fileName[partIndex], sizeof(fileName[partIndex]), FALSE);
            else
                fileName[partIndex][0]=0x00;
            
            RandomizeReadableString(content[partIndex], sizeof(content[partIndex]), FALSE);
            
            messageParts[partIndex].mimeType=mimeType[partIndex];
            messageParts[partIndex].mimeTypeLen=strlen(mimeType[partIndex]);
            messageParts[partIndex].fileName=fileName[partIndex];
            messageParts[partIndex].fileNameLen=strlen(fileName[partIndex]);
            messageParts[partIndex].content=(unsigned char*)content[partIndex];
            messageParts[partIndex].contentLen=strlen(content[partIndex]);
        }
        
        CoinSparkCalcMessageHash((unsigned char*)salt, strlen(salt), messageParts, countParts, messageHash);
        
        if (inputFile) {
            fprintf(inputFile, "%s # salt\n%d # parts\n", salt, countParts);
            
            for (partIndex=0; partIndex<countParts; partIndex++)
                fprintf(inputFile, "%s # part %d mime type\n%s # part %d file name\n%s # part %d content\n",
                        mimeType[partIndex], partIndex, fileName[partIndex], partIndex, content[partIndex], partIndex);
            
            fputc('\n', inputFile);
        }
        
        if (outputFile) {
            DisplayHexadecimal(messageHash, sizeof(messageHash), outputFile);
            fputc('\n', outputFile);
        }
        
        FlushTestFiles(inputFile, outputFile);
    }
    
    FinishTests(inputFile, outputFile, testName);
    
    return TRUE;
}

int main(int argc, const char* argv[])
{
	time_t timeStamp;
    struct tm timeComponents;
    char buffer[256], directoryName[256], *newLinePtr;
    int testSuite, countTests;
    bool toDisplay, toFiles;

    srand((unsigned int)time(NULL)); // if this is commented, the tests will be deterministic (at least on Mac OS X)

//  Ask the user which tests to perform
    
    printf("CoinSpark random tests\n");
    printf("----------------------\n\n");
    printf("[A]ddresses\n");
    printf("Asset [R]eferences\n");
    printf("[S]cript metadata\n");
    printf("Asset [H]ashes\n");
    printf("[G]enesis calculations\n");
    printf("[T]ransfer calculations\n");
    printf("[M]essage hashes\n");
    printf("\nChoose a test suite to run [all]: ");
    if (fgets(buffer, sizeof(buffer), stdin)) {
        newLinePtr=strchr(buffer, '\n');
        if (newLinePtr)
            *newLinePtr=0x00;
        
        testSuite=tolower(buffer[0]);
    } else
        return EXIT_FAILURE;
    
//  Ask the user how many tests
    
    countTests=10000;
    printf("\nNumber of tests [%d]: ", countTests);
    if (fgets(buffer, sizeof(buffer), stdin))
        sscanf(buffer, "%d", &countTests);
    
//  Ask the user whether to display the tests and/or store in files
    
    toDisplay=FALSE;
    printf("\nOutput tests to display [n]: ");
    if (fgets(buffer, sizeof(buffer), stdin) && (tolower(buffer[0])=='y'))
        toDisplay=TRUE;
    
    toFiles=TRUE;
    printf("\nOutput tests to file [y]: ");
    if (fgets(buffer, sizeof(buffer), stdin) && (tolower(buffer[0])=='n'))
        toFiles=FALSE;
    
//  If outputting to files, ask the user for directory and create it
    
    if (toFiles) {
        time(&timeStamp);
        timeComponents=*localtime(&timeStamp);
        sprintf(directoryName, "CoinSpark-Tests-%04d-%02d-%02d-%02d-%02d-%02d", 1900+timeComponents.tm_year, 1+timeComponents.tm_mon,
                timeComponents.tm_mday, timeComponents.tm_hour, timeComponents.tm_min, timeComponents.tm_sec);
        
        printf("\nDirectory name for tests [%s]: ", directoryName);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            newLinePtr=strchr(buffer, '\n');
            if (newLinePtr)
                *newLinePtr=0x00;
            
            if (*buffer)
                strcpy(directoryName, buffer);
        }

        printf("\nCreating directory %s\n", directoryName);
        
        if (mkdir(directoryName, 0755)<0) { // value of -1 indicates error
            printf("Could not create directory!\n\n");
            return EXIT_FAILURE;
        }
    }
    
//  Perform the tests
    
    printf("\n");
    
    if ((testSuite=='a') || (testSuite==0x00))
        if (!PerformAddressTests(toFiles ? directoryName : NULL, countTests, toDisplay))
            goto testFailed;

    if ((testSuite=='r') || (testSuite==0x00))
        if (!PerformAssetRefTests(toFiles ? directoryName : NULL, countTests, toDisplay))
            goto testFailed;
    
    if ((testSuite=='s') || (testSuite==0x00))
        if (!PerformScriptTests(toFiles ? directoryName : NULL, countTests, toDisplay))
            goto testFailed;
    
    if ((testSuite=='h') || (testSuite==0x00))
        if (!PerformAssetHashTests(toFiles ? directoryName : NULL, countTests, toDisplay))
            goto testFailed;
    
    if ((testSuite=='g') || (testSuite==0x00))
        if (!PerformGenesisTests(toFiles ? directoryName : NULL, countTests, toDisplay))
            goto testFailed;
    
    if ((testSuite=='t') || (testSuite==0x00))
        if (!PerformTransferTests(toFiles ? directoryName : NULL, countTests, toDisplay))
            goto testFailed;

    if ((testSuite=='m') || (testSuite==0x00))
        if (!PerformMessageHashTests(toFiles ? directoryName : NULL, countTests, toDisplay))
            goto testFailed;

//  Finish up
    
    return EXIT_SUCCESS;

    testFailed:
    printf("Tests were not completed successfully!\n\n");
    return EXIT_FAILURE;
}
