/*
 * CoinSpark 1.0 - Javascript library
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


//	Quasi-constants for use by clients of the library

var COINSPARK_SATOSHI_QTY_MAX = 2100000000000000;
var COINSPARK_ASSET_QTY_MAX = 100000000000000;
var COINSPARK_PAYMENT_REF_MAX = 4503599627370495; // 2^52-1

var COINSPARK_GENESIS_QTY_MANTISSA_MIN = 1;
var COINSPARK_GENESIS_QTY_MANTISSA_MAX = 1000;
var COINSPARK_GENESIS_QTY_EXPONENT_MIN = 0;
var COINSPARK_GENESIS_QTY_EXPONENT_MAX = 11;
var COINSPARK_GENESIS_CHARGE_FLAT_MAX = 5000;
var COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN = 0;
var COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX = 100;
var COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX = 50;
var COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN = 0;
var COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX = 2;
var COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN = 0;
var COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX = 250;
var COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN = 32;
var COINSPARK_GENESIS_PAGE_PATH_MAX_LEN = 24;
var COINSPARK_GENESIS_HASH_MIN_LEN = 12;
var COINSPARK_GENESIS_HASH_MAX_LEN = 32;

var COINSPARK_ASSETREF_BLOCK_NUM_MAX = 4294967295;
var COINSPARK_ASSETREF_TX_OFFSET_MAX = 4294967295;
var COINSPARK_ASSETREF_TXID_PREFIX_LEN = 2;

var COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE = -1; // magic number for a default route

var COINSPARK_IO_INDEX_MAX = 65535;
	
var COINSPARK_ADDRESS_FLAG_ASSETS = 1;
var COINSPARK_ADDRESS_FLAG_PAYMENT_REFS = 2;
var COINSPARK_ADDRESS_FLAG_MASK = 0x7FFFFF; // 23 bits are currently usable


//	Quasi-constants for internal use only

var COINSPARK_UNSIGNED_BYTE_MAX = 0xFF;
var COINSPARK_UNSIGNED_2_BYTES_MAX = 0xFFFF;
var COINSPARK_UNSIGNED_3_BYTES_MAX = 0xFFFFFF;
var COINSPARK_UNSIGNED_4_BYTES_MAX = 4294967295;

var COINSPARK_METADATA_IDENTIFIER = "SPK";
var COINSPARK_METADATA_IDENTIFIER_LEN = 3;
var COINSPARK_LENGTH_PREFIX_MAX = 96;
var COINSPARK_GENESIS_PREFIX = 'g';
var COINSPARK_TRANSFERS_PREFIX = 't';
var COINSPARK_PAYMENTREF_PREFIX = 'r';

var COINSPARK_FEE_BASIS_MAX_SATOSHIS = 1000;

var COINSPARK_INTEGER_TO_BASE_58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

var COINSPARK_DOMAIN_PACKING_PREFIX_MASK = 0xC0;
var COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT = 6;
var COINSPARK_DOMAIN_PACKING_SUFFIX_MASK = 0x3F;
var COINSPARK_DOMAIN_PACKING_SUFFIX_MAX = 62;
var COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4 = 63;
var COINSPARK_DOMAIN_PACKING_IPv4_HTTPS = 0x40;

var COINSPARK_DOMAIN_PATH_ENCODE_BASE = 40;
var COINSPARK_DOMAIN_PATH_FALSE_END_CHAR = '<';
var COINSPARK_DOMAIN_PATH_TRUE_END_CHAR= '>';
var COINSPARK_DOMAIN_NAME_CHARS = "0123456789abcdefghijklmnopqrstuvwxyz-.<>";

var COINSPARK_DOMAIN_NAME_PREFIXES=[
	"",
	"www."
];

var COINSPARK_DOMAIN_NAME_SUFFIXES=[
	"",
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
];


//	General public functions for managing CoinSpark metadata and bitcoin transaction output scripts

function CoinSparkScriptToMetadata(scriptPubKey, scriptIsHex)
{
	var scriptPubKeyRaw=CoinSparkGetRawScript(scriptPubKey, scriptIsHex);
	var scriptPubKeyRawLen=scriptPubKeyRaw.length;
	var metadataLen=scriptPubKeyRawLen-2;
	
	if (
		(scriptPubKeyRawLen>2) &&
		(scriptPubKeyRaw[0] == 0x6a) &&
		(scriptPubKeyRaw[1] > 0) &&
		(scriptPubKeyRaw[1] <= 75) &&
		(scriptPubKeyRaw[1] == metadataLen)
	)
		return scriptPubKeyRaw.slice(2);
	
	return null;
}

function CoinSparkScriptsToMetadata(scriptPubKeys, scriptsAreHex)
{
	for (var scriptIndex=0; scriptIndex<scriptPubKeys.length; scriptIndex++) {
		var scriptPubKey=scriptPubKeys[scriptIndex];
		if (!CoinSparkScriptIsRegular(scriptPubKey, scriptsAreHex))
			return CoinSparkScriptToMetadata(scriptPubKey, scriptsAreHex);
	}
	
	return null;
}

function CoinSparkMetadataToScript(metadata, toHexScript)
{
	if (metadata.length<=75) {
		var scriptPubKey=[0x6a, metadata.length].concat(metadata);
		if (toHexScript)
			scriptPubKey=CoinSparkUint8ArrayToHex(scriptPubKey);
		
		return scriptPubKey;
	}
		
	return null;
}

function CoinSparkMetadataMaxAppendLen(metadata, metadataMaxLen)
{
	return Math.max(metadataMaxLen-(metadata.length+1-COINSPARK_METADATA_IDENTIFIER_LEN), 0);
}

function CoinSparkMetadataAppend(metadata, metadataMaxLen, appendMetadata)
{
	var lastMetadata=CoinSparkLocateMetadataRange(metadata, null); // check we can find last metadata
	if (!lastMetadata)
		return null;

	if (appendMetadata.length<(COINSPARK_METADATA_IDENTIFIER_LEN+1)) // check there is enough to check the prefix
		return null;
		
	if (CoinSparkUint8ArrayToASCII(appendMetadata.slice(0, COINSPARK_METADATA_IDENTIFIER_LEN))!=COINSPARK_METADATA_IDENTIFIER) // then check the prefix
		return null;
	
	// we don't check the character after the prefix in appendMetadata because it could itself be composite
	
	var needLength=metadata.length+appendMetadata.length-COINSPARK_METADATA_IDENTIFIER_LEN+1; // check there is enough space
	if (metadataMaxLen<needLength)
		return 0;
	
	var lastMetadataLen=lastMetadata.length+1; // include prefix
	var lastMetadataPos=metadata.length-lastMetadataLen;
	
	return metadata.slice(0, lastMetadataPos).concat([lastMetadataLen]).concat(metadata.slice(lastMetadataPos)).
		concat(appendMetadata.slice(COINSPARK_METADATA_IDENTIFIER_LEN));
}

function CoinSparkScriptIsRegular(scriptPubKey, scriptIsHex)
{
	var scriptPubKeyRaw=CoinSparkGetRawScript(scriptPubKey, scriptIsHex);
		
	return (scriptPubKeyRaw.length<1) || (scriptPubKeyRaw[0]!=0x6a);
}


//	Function for calculating asset hashes

function CoinSparkCalcAssetHash(name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent)
{
	var buffer=CoinSparkAssetHashFieldTrim(name)+"\x00";
	buffer+=CoinSparkAssetHashFieldTrim(issuer)+"\x00";
	buffer+=CoinSparkAssetHashFieldTrim(description)+"\x00";
	buffer+=CoinSparkAssetHashFieldTrim(units)+"\x00";
	buffer+=CoinSparkAssetHashFieldTrim(issueDate)+"\x00";
	buffer+=CoinSparkAssetHashFieldTrim(expiryDate)+"\x00";
	
	var interestRateToHash=Math.floor(((interestRate!==null) ? interestRate : 0)*1000000.0+0.5);
	var multipleToHash=Math.floor(((multiple!==null) ? multiple : 1)*1000000.0+0.5);
	
	buffer+=interestRateToHash+"\x00";
	buffer+=multipleToHash+"\x00";
	
	var array=CoinSparkStringToUint8ArrayUTF8(buffer).concat(contractContent).concat([0x00]);
	
	return CoinSparkUint8ArraySHA256(array);
}

function CoinSparkAssetHashFieldTrim(string)
{
	return string ? string.replace(/^[\x09\x0A\x0D\x20]+/, '').replace(/[\x09\x0A\x0D\x20]+$/, '') : '';
}


//	CoinSparkAddress class for managing CoinSpark addresses

function CoinSparkAddress()
{
	this.clear();
}

CoinSparkAddress.prototype=new CoinSparkBase();

CoinSparkAddress.prototype.COINSPARK_ADDRESS_PREFIX='s';
CoinSparkAddress.prototype.COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE=10;
CoinSparkAddress.prototype.COINSPARK_ADDRESS_CHAR_INCREMENT=13;

CoinSparkAddress.prototype.clear=function()
{
	this.bitcoinAddress='';
	this.addressFlags=0;
	this.paymentRef=new CoinSparkPaymentRef();
}

CoinSparkAddress.prototype.toString=function()
{
	var flagsToStrings=[
		[COINSPARK_ADDRESS_FLAG_ASSETS, "assets"],
		[COINSPARK_ADDRESS_FLAG_PAYMENT_REFS, "payment references"]
	];
	
	var buffer="COINSPARK ADDRESS\n";
	buffer+="  Bitcoin address: "+this.bitcoinAddress+"\n";
	buffer+="    Address flags: "+this.addressFlags;
	
	var flagOutput=false;
	
	for (var index=0; index<flagsToStrings.length; index++)
		if (this.addressFlags & flagsToStrings[index][0]) {
			buffer+=(flagOutput ? ", " : " [")+flagsToStrings[index][1];
			flagOutput=true;
		}
		
	buffer+=(flagOutput ? "]" : "")+"\n";
	
	buffer+="Payment reference: "+this.paymentRef.ref+"\n";
	buffer+="END COINSPARK ADDRESS\n\n";
	
	return buffer;
}

CoinSparkAddress.prototype.isValid=function()
{
	if ( (!this.isString(this.bitcoinAddress)) || !this.bitcoinAddress.length )
		return false;
	
	if ( ((this.addressFlags&COINSPARK_ADDRESS_FLAG_MASK) != this.addressFlags) || !this.isInteger(this.addressFlags) )
		return false;

	return this.paymentRef.isValid();
}

CoinSparkAddress.prototype.match=function(otherAddress)
{
	return (this.bitcoinAddress===otherAddress.bitcoinAddress) &&
		(this.addressFlags==otherAddress.addressFlags) && (this.paymentRef.match(otherAddress.paymentRef));
}

CoinSparkAddress.prototype.encode=function()
{
	if (!this.isValid())
		return null;
	
	var stringBase58=[];

//  Build up extra data for address flags

	var addressFlagChars=0;
	var testAddressFlags=this.addressFlags;
	
	while (testAddressFlags>0) {
		stringBase58[2+addressFlagChars]=testAddressFlags%58;
		testAddressFlags=Math.floor(testAddressFlags/58);
		addressFlagChars++;
	}
	
//	Build up extra data for payment reference

	var paymentRefChars=0;
	var testPaymentRef=this.paymentRef.ref;

	while (testPaymentRef>0) {
		stringBase58[2+addressFlagChars+paymentRefChars]=testPaymentRef%58;
		testPaymentRef=Math.floor(testPaymentRef/58);
		paymentRefChars++;
	}
	
//	Calculate and encode extra length

	var extraDataChars=addressFlagChars+paymentRefChars;
	var bitcoinAddressLen=this.bitcoinAddress.length;
	var stringLen=bitcoinAddressLen+2+extraDataChars;
	
	stringBase58[1]=addressFlagChars*this.COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE+paymentRefChars;
	
//  Convert the bitcoin address

	for (var charIndex=0; charIndex<bitcoinAddressLen; charIndex++) {
		var charValue=this.base58ToInteger(this.bitcoinAddress.charAt(charIndex));
		if (charValue===null)
			return null; // invalid base58 character
	
		charValue+=this.COINSPARK_ADDRESS_CHAR_INCREMENT;

		if (extraDataChars>0)
			charValue+=stringBase58[2+charIndex%extraDataChars];
	
		stringBase58[2+extraDataChars+charIndex]=charValue%58;
	}
	
//  Obfuscate first half of address using second half to prevent common prefixes

	var halfLength=Math.ceil(stringLen/2);
	for (charIndex=1; charIndex<halfLength; charIndex++) // exclude first character
		stringBase58[charIndex]=(stringBase58[charIndex]+stringBase58[stringLen-charIndex])%58;
		
//	Convert to base 58 and add prefix

	var string=this.COINSPARK_ADDRESS_PREFIX;
	for (charIndex=1; charIndex<stringLen; charIndex++)
		string+=COINSPARK_INTEGER_TO_BASE_58.charAt(stringBase58[charIndex]);

	return string;
}

CoinSparkAddress.prototype.decode=function(string)
{

//  Check for basic validity

	var stringLen=string.length;
	if (stringLen<2)
		return false;
		
	if (string.charAt(0)!=this.COINSPARK_ADDRESS_PREFIX)
		return false;
		
//	Convert from base 58

	var stringBase58=[];
	for (var charIndex=1; charIndex<stringLen; charIndex++) { // exclude first character
		var charValue=this.base58ToInteger(string.charAt(charIndex));
		if (charValue===null)
			return false;
		stringBase58[charIndex]=charValue;
	}
	
//	De-obfuscate first half of address using second half

	var halfLength=Math.ceil(stringLen/2);
	for (charIndex=1; charIndex<halfLength; charIndex++) // exclude first character
		stringBase58[charIndex]=(stringBase58[charIndex]+58-stringBase58[stringLen-charIndex])%58;
		
//	Get length of extra data

	charValue=stringBase58[1];
	var addressFlagChars=Math.floor(charValue/this.COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE);
	var paymentRefChars=charValue%this.COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE;
	var extraDataChars=addressFlagChars+paymentRefChars;
	
	if (stringLen<(2+extraDataChars))
		return false;
		
	var bitcoinAddressLen=stringLen-2-extraDataChars;
	
//  Read the extra data for address flags

	this.addressFlags=0;
	var multiplier=1;

	for (charIndex=0; charIndex<addressFlagChars; charIndex++) {
		charValue=stringBase58[2+charIndex];
		this.addressFlags+=charValue*multiplier;
		multiplier*=58;
	}
	
//	Read the extra data for payment reference

	this.paymentRef.ref=0;
	multiplier=1;
	
	for (charIndex=0; charIndex<paymentRefChars; charIndex++) {
		charValue=stringBase58[2+addressFlagChars+charIndex];
		this.paymentRef.ref+=charValue*multiplier;
		multiplier*=58;
	}
	
//  Convert the bitcoin address
	
	this.bitcoinAddress='';
	
	for (charIndex=0; charIndex<bitcoinAddressLen; charIndex++) {
		charValue=stringBase58[2+extraDataChars+charIndex];
		charValue+=58*2-this.COINSPARK_ADDRESS_CHAR_INCREMENT; // avoid worrying about the result of modulo on negative numbers in any language
	
		if (extraDataChars>0)
			charValue-=stringBase58[2+charIndex%extraDataChars];
	
		this.bitcoinAddress+=COINSPARK_INTEGER_TO_BASE_58.charAt(charValue%58);
	}
	
	return this.isValid();
}


//	CoinSparkGenesis class for managing asset genesis metadata

function CoinSparkGenesis()
{
	this.clear();
}

CoinSparkGenesis.prototype=new CoinSparkBase();

CoinSparkGenesis.prototype.COINSPARK_GENESIS_QTY_FLAGS_LENGTH=2;
CoinSparkGenesis.prototype.COINSPARK_GENESIS_QTY_MASK=0x3FFF;
CoinSparkGenesis.prototype.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE=1001;
CoinSparkGenesis.prototype.COINSPARK_GENESIS_FLAG_CHARGE_FLAT=0x4000;
CoinSparkGenesis.prototype.COINSPARK_GENESIS_FLAG_CHARGE_BPS=0x8000;
CoinSparkGenesis.prototype.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE=101;
CoinSparkGenesis.prototype.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH=1;
CoinSparkGenesis.prototype.COINSPARK_GENESIS_CHARGE_BPS_LENGTH=1;

CoinSparkGenesis.prototype.clear=function()
{
	this.qtyMantissa=0;
	this.qtyExponent=0;
	this.chargeFlatMantissa=0;
	this.chargeFlatExponent=0;
	this.chargeBasisPoints=0;
	this.useHttps=false;
	this.domainName='';
	this.usePrefix=true;
	this.pagePath='';
	this.assetHash=[];
	this.assetHashLen=0;
}

CoinSparkGenesis.prototype.toString=function()
{
	var quantity=this.getQty();
	var quantityEncoded=(this.qtyExponent*this.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+this.qtyMantissa)&this.COINSPARK_GENESIS_QTY_MASK;
	var chargeFlat=this.getChargeFlat();
	var chargeFlatEncoded=this.chargeFlatExponent*this.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+this.chargeFlatMantissa;
	var domainPathMetadata=this.encodeDomainAndOrPath(this.domainName, this.useHttps, this.pagePath, this.usePrefix);
	
	buffer="COINSPARK GENESIS\n";
	buffer+="   Quantity mantissa: "+this.qtyMantissa+"\n";
	buffer+="   Quantity exponent: "+this.qtyExponent+"\n";
	buffer+="    Quantity encoded: "+quantityEncoded+" (small endian hex "+
		this.unsignedToSmallEndianHex(quantityEncoded, this.COINSPARK_GENESIS_QTY_FLAGS_LENGTH)+")\n";
	buffer+="      Quantity value: "+quantity+"\n";
	buffer+="Flat charge mantissa: "+this.chargeFlatMantissa+"\n";
	buffer+="Flat charge exponent: "+this.chargeFlatExponent+"\n";
	buffer+=" Flat charge encoded: "+chargeFlatEncoded+" (small endian hex "+
		this.unsignedToSmallEndianHex(chargeFlatEncoded, this.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH)+")\n";
	buffer+="   Flat charge value: "+chargeFlat+"\n";
	buffer+=" Basis points charge: "+this.chargeBasisPoints+" (hex "+
		this.unsignedToSmallEndianHex(this.chargeBasisPoints, this.COINSPARK_GENESIS_CHARGE_BPS_LENGTH)+")\n";
	buffer+="           Asset URL: "+(this.useHttps ? 'https' : 'http')+"://"+this.domainName+"/"+
		(this.usePrefix ?  "coinspark/" : "")+(this.pagePath.length ? this.pagePath : "[spent-txid]")+
		"/ (length "+this.domainName.length+"+"+this.pagePath.length+" encoded "+
		CoinSparkUint8ArrayToHex(domainPathMetadata)+" length "+domainPathMetadata.length+")\n",
	buffer+="          Asset hash: "+CoinSparkUint8ArrayToHex(this.assetHash.slice(0, this.assetHashLen))+
		" (length "+this.assetHashLen+")\n";
	buffer+="END COINSPARK GENESIS\n\n";
	
	return buffer;
}

CoinSparkGenesis.prototype.isValid=function()
{
	if (!(
		this.isInteger(this.qtyMantissa) &&
		this.isInteger(this.qtyExponent) &&
		this.isInteger(this.chargeFlatMantissa) &&
		this.isInteger(this.chargeFlatExponent) &&
		this.isInteger(this.chargeBasisPoints) &&
		this.isBoolean(this.useHttps) &&
		this.isString(this.domainName) &&
		this.isBoolean(this.usePrefix) &&
		this.isString(this.pagePath) &&
		this.isUInt8Array(this.assetHash) &&
		this.isInteger(this.assetHashLen)
	))
		return false;
	
	if ( (this.qtyMantissa<COINSPARK_GENESIS_QTY_MANTISSA_MIN) || (this.qtyMantissa>COINSPARK_GENESIS_QTY_MANTISSA_MAX) )
		return false;
		
	if ( (this.qtyExponent<COINSPARK_GENESIS_QTY_EXPONENT_MIN) || (this.qtyExponent>COINSPARK_GENESIS_QTY_EXPONENT_MAX) )
		return false;
	
	if ( (this.chargeFlatExponent<COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN) || (this.chargeFlatExponent>COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) )
		return false;
		
	if (this.chargeFlatMantissa<COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN)
		return false;
		
	if (this.chargeFlatMantissa > ((this.chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) ? COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX : COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX))
		return false;
		
	if ( (this.chargeBasisPoints<COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN) || (this.chargeBasisPoints>COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX) )
		return false;
		
	if (this.domainName.length>COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN)
		return false;
		
	if (this.pagePath.length>COINSPARK_GENESIS_PAGE_PATH_MAX_LEN)
		return false;
		
	if (this.assetHash.length<this.assetHashLen) // check we have at least as much data as specified by this.assetHashLen
		return false; 
	
	if ( (this.assetHashLen<COINSPARK_GENESIS_HASH_MIN_LEN) || (this.assetHashLen>COINSPARK_GENESIS_HASH_MAX_LEN) )
		return false;
		
	return true;
}

CoinSparkGenesis.prototype.match=function(otherGenesis, strict)
{
	var hashCompareLen=Math.min(this.assetHashLen, otherGenesis.assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN);
	
	if (strict)
		var floatQuantitiesMatch=(this.qtyMantissa==otherGenesis.qtyMantissa) && (this.qtyExponent==otherGenesis.qtyExponent) &&
			(this.chargeFlatMantissa==otherGenesis.chargeFlatMantissa) && (this.chargeFlatExponent==otherGenesis.chargeFlatExponent);
	else
		var floatQuantitiesMatch=(this.getQty()==otherGenesis.getQty()) && (this.getChargeFlat()==otherGenesis.getChargeFlat());
	
	return floatQuantitiesMatch && (this.chargeBasisPoints==otherGenesis.chargeBasisPoints) &&
		(this.useHttps==otherGenesis.useHttps) &&
		(this.domainName.toLowerCase()==otherGenesis.domainName.toLowerCase()) &&
		(this.usePrefix==otherGenesis.usePrefix) &&
		(this.pagePath.toLowerCase()==otherGenesis.pagePath.toLowerCase()) &&
		(this.assetHash.slice(0, hashCompareLen).toString()==otherGenesis.assetHash.slice(0, hashCompareLen).toString());
}

CoinSparkGenesis.prototype.getQty=function()
{
	return this.mantissaExponentToQty(this.qtyMantissa, this.qtyExponent);
}

CoinSparkGenesis.prototype.setQty=function(desiredQty, rounding)
{
	var result=this.qtyToMantissaExponent(desiredQty, rounding, COINSPARK_GENESIS_QTY_MANTISSA_MAX, COINSPARK_GENESIS_QTY_EXPONENT_MAX);

	this.qtyMantissa=result.mantissa;
	this.qtyExponent=result.exponent;
	
	return this.getQty();
}

CoinSparkGenesis.prototype.getChargeFlat=function()
{
	return this.mantissaExponentToQty(this.chargeFlatMantissa, this.chargeFlatExponent);
}

CoinSparkGenesis.prototype.setChargeFlat=function(desiredChargeFlat, rounding)
{
	var result=this.qtyToMantissaExponent(desiredChargeFlat, rounding,
		COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX, COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX);
	
	this.chargeFlatMantissa=result.mantissa;
	this.chargeFlatExponent=result.exponent;
	
	if (this.chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX)
		this.chargeFlatMantissa=Math.min(this.chargeFlatMantissa, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX);
		
	return this.getChargeFlat();
}

CoinSparkGenesis.prototype.calcCharge=function(qtyGross)
{
	var charge=this.getChargeFlat()+Math.floor((qtyGross*this.chargeBasisPoints+5000)/10000); // rounds to nearest
	
	return Math.min(qtyGross, charge);
}

CoinSparkGenesis.prototype.calcNet=function(qtyGross)
{
	return qtyGross-this.calcCharge(qtyGross);
}

CoinSparkGenesis.prototype.calcGross=function(qtyNet)
{
	qtyNet=parseInt(qtyNet); // to prevent + performing concatenation
	
	if (qtyNet<=0)
		return 0; // no point getting past charges if we end up with zero anyway
		
	var lowerGross=Math.floor(((qtyNet+this.getChargeFlat())*10000)/(10000-this.chargeBasisPoints)); // divides rounding down
	
	return (this.calcNet(lowerGross)>=qtyNet) ? lowerGross : (lowerGross+1);
}

CoinSparkGenesis.prototype.calcHashLen=function(metadataMaxLen)
{
	var assetHashLen=metadataMaxLen-COINSPARK_METADATA_IDENTIFIER_LEN-1-this.COINSPARK_GENESIS_QTY_FLAGS_LENGTH;
	
	if (this.chargeFlatMantissa>0)
		assetHashLen-=this.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH;
		
	if (this.chargeBasisPoints>0)
		assetHashLen-=this.COINSPARK_GENESIS_CHARGE_BPS_LENGTH;
		
	var domainPathLen=this.pagePath.length+1;
		
	if (this.readIPv4Address(this.domainName))
		assetHashLen-=5; // packing and IP octets
	else {
		assetHashLen-=1; // packing
		domainPathLen+=this.shrinkLowerDomainName(this.domainName).domainName.length+1;
	}

	assetHashLen-=2*Math.floor((domainPathLen+2)/3); // uses integer arithmetic
	
	return Math.min(assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN);
}

CoinSparkGenesis.prototype.encode=function(metadataMaxLen)
{
	if (!this.isValid())
		return null;
	
//  4-character identifier

	var metadata=CoinSparkASCIIToUint8Array(COINSPARK_METADATA_IDENTIFIER+COINSPARK_GENESIS_PREFIX);

//	Quantity mantissa and exponent

	var quantityEncoded=(this.qtyExponent*this.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+this.qtyMantissa)&this.COINSPARK_GENESIS_QTY_MASK;
	if (this.chargeFlatMantissa>0)
		quantityEncoded|=this.COINSPARK_GENESIS_FLAG_CHARGE_FLAT;
	if (this.chargeBasisPoints>0)
		quantityEncoded|=this.COINSPARK_GENESIS_FLAG_CHARGE_BPS;

	var array=this.writeSmallEndianUnsigned(quantityEncoded, this.COINSPARK_GENESIS_QTY_FLAGS_LENGTH);
	if (!array)
		return null;
	
	metadata=metadata.concat(array);
	
//	Charges - flat and basis points

	if (quantityEncoded & this.COINSPARK_GENESIS_FLAG_CHARGE_FLAT) {
		var chargeEncoded=this.chargeFlatExponent*this.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+this.chargeFlatMantissa;
	
		array=this.writeSmallEndianUnsigned(chargeEncoded, this.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH);
		if (!array)
			return null;
			
		metadata=metadata.concat(array);
	}

	if (quantityEncoded & this.COINSPARK_GENESIS_FLAG_CHARGE_BPS) {
		array=this.writeSmallEndianUnsigned(this.chargeBasisPoints, this.COINSPARK_GENESIS_CHARGE_BPS_LENGTH);
		if (!array)
			return null;
			
		metadata=metadata.concat(array);
	}
	
//	Domain name and page path

	array=this.encodeDomainAndOrPath(this.domainName, this.useHttps, this.pagePath, this.usePrefix);
	if (!array)
		return null;

	metadata=metadata.concat(array);

//	Asset hash
	
	metadata=metadata.concat(this.assetHash.slice(0, this.assetHashLen));
	
//	Check the total length is within the specified limit

	if (metadata.length>metadataMaxLen)
		return null;
		
//	Return what we created

	return metadata;
}

CoinSparkGenesis.prototype.decode=function(metadata)
{
	metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_GENESIS_PREFIX);
	if (!metadata)
		return false;
	
//	Quantity mantissa and exponent

	var quantityEncoded=this.shiftReadSmallEndianUnsigned(metadata, this.COINSPARK_GENESIS_QTY_FLAGS_LENGTH);
	if (quantityEncoded==null)
		return false;
		
	this.qtyMantissa=(quantityEncoded&this.COINSPARK_GENESIS_QTY_MASK)%this.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE;
	this.qtyExponent=Math.floor((quantityEncoded&this.COINSPARK_GENESIS_QTY_MASK)/this.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE);

//	Charges - flat and basis points

	if (quantityEncoded & this.COINSPARK_GENESIS_FLAG_CHARGE_FLAT) {
		var chargeEncoded=this.shiftReadSmallEndianUnsigned(metadata, this.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH);
		
		this.chargeFlatMantissa=chargeEncoded%this.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE;
		this.chargeFlatExponent=Math.floor(chargeEncoded/this.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE);
	
	} else {
		this.chargeFlatMantissa=0;
		this.chargeFlatExponent=0;
	}
	
	if (quantityEncoded & this.COINSPARK_GENESIS_FLAG_CHARGE_BPS)
		this.chargeBasisPoints=this.shiftReadSmallEndianUnsigned(metadata, this.COINSPARK_GENESIS_CHARGE_BPS_LENGTH);
	else
		this.chargeBasisPoints=0;

//	Domain name and page path
	
	var decodedDomainPath=this.shiftDecodeDomainAndOrPath(metadata, true, true);
	if (!decodedDomainPath)
		return false;
		
	this.useHttps=decodedDomainPath.useHttps;
	this.domainName=decodedDomainPath.domainName;
	this.usePrefix=decodedDomainPath.usePrefix;
	this.pagePath=decodedDomainPath.pagePath;

//	Asset hash

	this.assetHashLen=Math.min(metadata.length, COINSPARK_GENESIS_HASH_MAX_LEN);
	this.assetHash=metadata.splice(0, this.assetHashLen);
	
//	Return validity

	return this.isValid();
}

CoinSparkGenesis.prototype.calcMinFee=function(outputsSatoshis, outputsRegular)
{
	if (outputsSatoshis.length!=outputsRegular.length)
		return COINSPARK_SATOSHI_QTY_MAX; // these two arrays must be the same size

	return this.countNonLastRegularOutputs(outputsRegular)*this.getMinFeeBasis(outputsSatoshis, outputsRegular);
}

CoinSparkGenesis.prototype.apply=function(outputsRegular)
{
	var countOutputs=outputsRegular.length;
	var lastRegularOutput=this.getLastRegularOutput(outputsRegular);
	var divideOutputs=this.countNonLastRegularOutputs(outputsRegular);
	var genesisQty=this.getQty();
	
	if (divideOutputs==0)
		var qtyPerOutput=0;
	else
		var qtyPerOutput=Math.floor(genesisQty/divideOutputs); // rounds down
		
	var extraFirstOutput=genesisQty-qtyPerOutput*divideOutputs;
	var outputBalances=CoinSparkArrayFill(0, countOutputs, 0);
	
	for (var outputIndex=0; outputIndex<countOutputs; outputIndex++)
		if (outputsRegular[outputIndex] && (outputIndex!=lastRegularOutput)) {
			outputBalances[outputIndex]=qtyPerOutput+extraFirstOutput;
			extraFirstOutput=0; // so it will only contribute to the first
		}
	
	return outputBalances;
}

CoinSparkGenesis.prototype.calcAssetURL=function(firstSpentTxID, firstSpentVout)
{
	var firstSpentTxIdPart=(firstSpentTxID+firstSpentTxID).substr(firstSpentVout%64, 16);
	
	return (
		((this.useHttps) ? 'https' : 'http')+
		'://'+this.domainName+'/'+
		(this.usePrefix ? 'coinspark/' : '')+
		(this.pagePath.length ? this.pagePath : firstSpentTxIdPart)+'/'
	).toLowerCase();
}


//	CoinSparkAssetRef class for managing asset references

function CoinSparkAssetRef()
{
	this.clear();
}

CoinSparkAssetRef.prototype=new CoinSparkBase();

CoinSparkAssetRef.prototype.clear=function()
{
	this.blockNum=0;
	this.txOffset=0;
	this.txIDPrefix="0000000000000000".substr(0, 2*COINSPARK_ASSETREF_TXID_PREFIX_LEN);
}

CoinSparkAssetRef.prototype.toString=function()
{
	return this.toStringInner(true);
}

CoinSparkAssetRef.prototype.isValid=function()
{
	if (this.blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) {
		if ( (this.blockNum<0) || (this.blockNum>COINSPARK_ASSETREF_BLOCK_NUM_MAX) || !this.isInteger(this.blockNum) )
			return false;
	
		if ( (this.txOffset<0) || (this.txOffset>COINSPARK_ASSETREF_TX_OFFSET_MAX) || !this.isInteger(this.txOffset) )
			return false;
		
		if ( (!this.isString(this.txIDPrefix)) || (this.txIDPrefix.length!=(2*COINSPARK_ASSETREF_TXID_PREFIX_LEN)) )
			return false;
		
		if (parseInt(this.txIDPrefix, 16).toString(16).replace(/^0+/,'').toUpperCase()!=this.txIDPrefix.replace(/^0+/,'').toUpperCase())
			return false;
	}
		
	return true;
}

CoinSparkAssetRef.prototype.match=function(otherAssetRef)
{
	return (this.txIDPrefix.toLowerCase()==otherAssetRef.txIDPrefix.toLowerCase()) &&
		(this.txOffset == otherAssetRef.txOffset) && (this.blockNum == otherAssetRef.blockNum);
}

CoinSparkAssetRef.prototype.encode=function()
{
	if (!this.isValid())
		return null;
	
	var txIDPrefixInteger=256*parseInt(this.txIDPrefix.substr(2, 2), 16)+parseInt(this.txIDPrefix.substr(0, 2), 16);
	
	return this.blockNum+'-'+this.txOffset+'-'+txIDPrefixInteger;
}

CoinSparkAssetRef.prototype.decode=function(string)
{
	if (string.match(/[^0-9\-]/)) // check for illegal characters
		return false;
	
	var parts=string.split('-');
	if (parts.length!=3) // check right number of parts
		return false;
		
	var txIDPrefixInteger=parts[2];	
	if ( (txIDPrefixInteger<0) || (txIDPrefixInteger>0xFFFF) )
		return false;
		
	this.blockNum=parts[0];
	this.txOffset=parts[1];
	this.txIDPrefix=this.unsignedToSmallEndianHex(txIDPrefixInteger, 2);
	
	return this.isValid();
}

CoinSparkAssetRef.prototype.toStringInner=function(headers)
{
	var buffer=headers ? "COINSPARK ASSET REFERENCE\n" : "";
	
	buffer+="Genesis block index: "+this.blockNum+" (small endian hex "+this.unsignedToSmallEndianHex(this.blockNum, 4)+")\n";
	buffer+=" Genesis txn offset: "+this.txOffset+" (small endian hex "+this.unsignedToSmallEndianHex(this.txOffset, 4)+")\n";
	buffer+="Genesis txid prefix: "+this.txIDPrefix.toUpperCase()+"\n";
	
	if (headers)
		buffer+="END COINSPARK ASSET REFERENCE\n\n";
		
	return buffer;
}

CoinSparkAssetRef.prototype.compare=function(otherAssetRef)
{
	// -1 if this<otherAssetRef, 1 if otherAssetRef>this, 0 otherwise
	
	if (this.blockNum!=otherAssetRef.blockNum)
		return (this.blockNum<otherAssetRef.blockNum) ? -1 : 1;
	else if (this.blocNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) // in this case don't compare other fields
		return 0;
	else if (this.txOffset!=otherAssetRef.txOffset)
		return (this.txOffset<otherAssetRef.txOffset) ? -1 : 1;
	else {
		var thisTxIDPrefixLower=this.txIDPrefix.substr(0, 2*COINSPARK_ASSETREF_TXID_PREFIX_LEN).toLowerCase();
		var otherTxIDPrefixLower=otherAssetRef.txIDPrefix.substr(0, 2*COINSPARK_ASSETREF_TXID_PREFIX_LEN).toLowerCase();
		
		if (thisTxIDPrefixLower!=otherTxIDPrefixLower) // comparing hex gives same order as comparing bytes
			return (thisTxIDPrefixLower<otherTxIDPrefixLower) ? -1 : 1;
		else
			return 0;
	}
}


//	CoinSparkTransfer class for managing individual asset transfer metadata

function CoinSparkTransfer()
{
	this.clear();
}

CoinSparkTransfer.prototype=new CoinSparkBase();

CoinSparkTransfer.prototype.COINSPARK_PACKING_GENESIS_MASK=0xC0;
CoinSparkTransfer.prototype.COINSPARK_PACKING_GENESIS_PREV=0x00;
CoinSparkTransfer.prototype.COINSPARK_PACKING_GENESIS_3_3_BYTES=0x40; // 3 bytes for block index, 3 for txn offset
CoinSparkTransfer.prototype.COINSPARK_PACKING_GENESIS_3_4_BYTES=0x80; // 3 bytes for block index, 4 for txn offset
CoinSparkTransfer.prototype.COINSPARK_PACKING_GENESIS_4_4_BYTES=0xC0; // 4 bytes for block index, 4 for txn offset

CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_MASK=0x38;
CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_0P_0P=0x00; // input 0 only or previous, output 0 only or previous
CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_0P_1S=0x08; // input 0 only or previous, output 1 only or subsequent single
CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_0P_ALL=0x10; // input 0 only or previous, all outputs
CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_1S_0P=0x18; // input 1 only or subsequent single, output 0 only or previous
CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_ALL_0P=0x20; // all inputs, output 0 only or previous
CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_ALL_1S=0x28; // all inputs, output 1 only or subsequent single
CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_ALL_ALL=0x30; // all inputs, all outputs
CoinSparkTransfer.prototype.COINSPARK_PACKING_INDICES_EXTEND=0x38; // use second byte for more extensive information

CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT=3;
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT=0;

CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_MASK=0x07;
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_0P=0x00; // index 0 only or previous
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_1S=0x01; // index 1 only or subsequent single
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_BYTE=0x02; // 1 byte for single index
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_2_BYTES=0x03; // 2 bytes for single index
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_1_1_BYTES=0x04; // 1 byte for first index, 1 byte for count
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_2_1_BYTES=0x05; // 2 bytes for first index, 1 byte for count
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_2_2_BYTES=0x06; // 2 bytes for first index, 2 bytes for count
CoinSparkTransfer.prototype.COINSPARK_PACKING_EXTEND_ALL=0x07; // all inputs|outputs

CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_MASK=0x07;
CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_1P=0x00; // quantity=1 or previous
CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_1_BYTE=0x01;
CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_2_BYTES=0x02;
CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_3_BYTES=0x03;
CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_4_BYTES=0x04;
CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_6_BYTES=0x05;
CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_FLOAT=0x06;
CoinSparkTransfer.prototype.COINSPARK_PACKING_QUANTITY_MAX=0x07; // transfer all quantity across

CoinSparkTransfer.prototype.COINSPARK_TRANSFER_QTY_FLOAT_LENGTH=2;
CoinSparkTransfer.prototype.COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX=1000;
CoinSparkTransfer.prototype.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX=11;
CoinSparkTransfer.prototype.COINSPARK_TRANSFER_QTY_FLOAT_MASK=0x3FFF;
CoinSparkTransfer.prototype.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE=1001;

CoinSparkTransfer.prototype.clear=function()
{
	this.assetRef=new CoinSparkAssetRef();
	this.inputs=new CoinSparkInOutRange();
	this.outputs=new CoinSparkInOutRange();
	this.qtyPerOutput=0;
}

CoinSparkTransfer.prototype.toString=function()
{
	return this.toStringInner(true);
}

CoinSparkTransfer.prototype.isValid=function()
{
	if (!(this.assetRef.isValid() && this.inputs.isValid() && this.outputs.isValid()))
		return false;
		
	if ( (this.qtyPerOutput<0) || (this.qtyPerOutput>COINSPARK_ASSET_QTY_MAX) || !this.isInteger(this.qtyPerOutput) )
		return false;
		
	return true;
}

CoinSparkTransfer.prototype.match=function(otherTransfer)
{
	if (this.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
		return (otherTransfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) &&
			this.inputs.match(otherTransfer.inputs) & (this.outputs.first==otherTransfer.outputs.first);
	
	else
		return this.assetRef.match(otherTransfer.assetRef) &&
			this.inputs.match(otherTransfer.inputs) &&
			this.outputs.match(otherTransfer.outputs) &&
			this.qtyPerOutput==otherTransfer.qtyPerOutput;
}

CoinSparkTransfer.prototype.encode=function(previousTransfer, metadataMaxLen, countInputs, countOutputs)
{
	if (!this.isValid())
		return null;
	
	var packing=0;
	var packingExtend=0;
	var isDefaultRoute=(this.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);
	
//  Packing for genesis reference
	
	if (isDefaultRoute) {
		if (previousTransfer && (previousTransfer.assetRef.blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE))
			return null; // default route transfers have to come at the start
			
		packing|=this.COINSPARK_PACKING_GENESIS_PREV;
	
	} else {
		if (previousTransfer && this.assetRef.match(previousTransfer.assetRef))
			packing|=this.COINSPARK_PACKING_GENESIS_PREV;
		
		else if (this.assetRef.blockNum <= COINSPARK_UNSIGNED_3_BYTES_MAX) {
			if (this.assetRef.txOffset <= COINSPARK_UNSIGNED_3_BYTES_MAX)
				packing|=this.COINSPARK_PACKING_GENESIS_3_3_BYTES;
			else if (this.assetRef.txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX)
				packing|=this.COINSPARK_PACKING_GENESIS_3_4_BYTES;
			else
				return null;

		} else if ( (this.assetRef.blockNum <= COINSPARK_UNSIGNED_4_BYTES_MAX) && (this.assetRef.txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX) )
			packing|=this.COINSPARK_PACKING_GENESIS_4_4_BYTES;
		
		else
			return null;
	}
		
//	Packing for input and output indices

	var inputPackingOptions=this.getPackingOptions(previousTransfer ? previousTransfer.inputs : null, this.inputs, countInputs);
	var outputPackingOptions=this.getPackingOptions(previousTransfer ? previousTransfer.outputs : null, this.outputs, countOutputs);
	
	if (inputPackingOptions['_0P'] && outputPackingOptions['_0P'])
		packing|=this.COINSPARK_PACKING_INDICES_0P_0P;
	else if (inputPackingOptions['_0P'] && outputPackingOptions['_1S'])
		packing|=this.COINSPARK_PACKING_INDICES_0P_1S;
	else if (inputPackingOptions['_0P'] && outputPackingOptions['_ALL'])
		packing|=this.COINSPARK_PACKING_INDICES_0P_ALL;
	else if (inputPackingOptions['_1S'] && outputPackingOptions['_0P'])
		packing|=this.COINSPARK_PACKING_INDICES_1S_0P;
	else if (inputPackingOptions['_ALL'] && outputPackingOptions['_0P'])
		packing|=this.COINSPARK_PACKING_INDICES_ALL_0P;
	else if (inputPackingOptions['_ALL'] && outputPackingOptions['_1S'])
		packing|=this.COINSPARK_PACKING_INDICES_ALL_1S;
	else if (inputPackingOptions['_ALL'] && outputPackingOptions['_ALL'])
		packing|=this.COINSPARK_PACKING_INDICES_ALL_ALL;

	else { // we need the second (extended) packing byte
		packing|=this.COINSPARK_PACKING_INDICES_EXTEND;

		var packingExtendInput=this.encodePackingExtend(inputPackingOptions);
		var packingExtendOutput=this.encodePackingExtend(outputPackingOptions);

		if ( (packingExtendInput===null) || (packingExtendOutput===null) )
			return null;
			
		var packingExtend=(packingExtendInput << this.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) | (packingExtendOutput << this.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT);
	}
	
//	Packing for quantity
	
	var encodeQuantity=this.qtyPerOutput;
   
	if (this.qtyPerOutput==(previousTransfer ? previousTransfer.qtyPerOutput : 1))
		packing|=this.COINSPARK_PACKING_QUANTITY_1P;
	else if (this.qtyPerOutput>=COINSPARK_ASSET_QTY_MAX)
		packing|=this.COINSPARK_PACKING_QUANTITY_MAX;
	else if (this.qtyPerOutput<=COINSPARK_UNSIGNED_BYTE_MAX)
		packing|=this.COINSPARK_PACKING_QUANTITY_1_BYTE;
	else if (this.qtyPerOutput<=COINSPARK_UNSIGNED_2_BYTES_MAX)
		packing|=this.COINSPARK_PACKING_QUANTITY_2_BYTES;
	else {
		var result=this.qtyToMantissaExponent(this.qtyPerOutput, 0,
			this.COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX, this.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX);
			
		if (result.qty==this.qtyPerOutput) {
			packing|=this.COINSPARK_PACKING_QUANTITY_FLOAT;
			encodeQuantity=(result.exponent*this.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+result.mantissa)&this.COINSPARK_TRANSFER_QTY_FLOAT_MASK;
		
		} else if (this.qtyPerOutput<=COINSPARK_UNSIGNED_3_BYTES_MAX)
			packing|=this.COINSPARK_PACKING_QUANTITY_3_BYTES;
		else if (this.qtyPerOutput<=COINSPARK_UNSIGNED_4_BYTES_MAX)
			packing|=this.COINSPARK_PACKING_QUANTITY_4_BYTES;
		else
			packing|=this.COINSPARK_PACKING_QUANTITY_6_BYTES;
	}
		
//	Write out the actual data

	var counts=this.packingToByteCounts(packing, packingExtend);
	
	metadata=[packing];
	
	if ( (packing & this.COINSPARK_PACKING_INDICES_MASK) == this.COINSPARK_PACKING_INDICES_EXTEND)
		metadata.push(packingExtend);
	
	var written_array=[
		this.writeUnsignedField(counts['blockNumBytes'], this.assetRef.blockNum),
		this.writeUnsignedField(counts['txOffsetBytes'], this.assetRef.txOffset),
		CoinSparkHexToUint8Array(this.assetRef.txIDPrefix).concat([0, 0, 0, 0, 0, 0, 0, 0]).slice(0, counts['txIDPrefixBytes']), // ensure right length
		this.writeUnsignedField(counts['firstInputBytes'], this.inputs.first),
		this.writeUnsignedField(counts['countInputsBytes'], this.inputs.count),
		this.writeUnsignedField(counts['firstOutputBytes'], this.outputs.first),
		this.writeUnsignedField(counts['countOutputsBytes'], this.outputs.count),
		this.writeUnsignedField(counts['quantityBytes'], encodeQuantity)
	];
	
	for (var writtenIndex=0; writtenIndex<written_array.length; writtenIndex++)
		if (written_array[writtenIndex]===null)
			return null;
		else
			metadata=metadata.concat(written_array[writtenIndex]);
			
//	Check the total length is within the specified limit

	if (metadata.length>metadataMaxLen)
		return null;
		
//	Return what we created

	return metadata;			
}

CoinSparkTransfer.prototype.decode=function(_metadata, previousTransfer, countInputs, countOutputs)
{
	var metadata=_metadata.slice(0); // clones the input array
	var startLength=metadata.length;
	
//  Extract packing

	var packing=metadata.shift();
	if (typeof packing == 'undefined')
		return 0;
	var packingExtend=0;
		
//  Packing for genesis reference

	switch (packing & this.COINSPARK_PACKING_GENESIS_MASK)
	{
		case this.COINSPARK_PACKING_GENESIS_PREV:
			if (previousTransfer)
				this.assetRef=previousTransfer.assetRef;
				
			else { // it's for a default route
				this.assetRef.blockNum=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE;
				this.assetRef.txOffset=0;
				this.assetRef.txIDPrefix="0000000000000000".substr(0, 2*COINSPARK_ASSETREF_TXID_PREFIX_LEN);
			}
			break;
	}

//  Packing for input and output indices

	if ((packing & this.COINSPARK_PACKING_INDICES_MASK) == this.COINSPARK_PACKING_INDICES_EXTEND) { // we're using second packing metadata byte
		var packingExtend=metadata.shift();
		if (typeof packingExtend == 'undefined')
			return 0;
			
		var inputPackingType=this.decodePackingExtend((packingExtend >> this.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & this.COINSPARK_PACKING_EXTEND_MASK);
		var outputPackingType=this.decodePackingExtend((packingExtend >> this.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & this.COINSPARK_PACKING_EXTEND_MASK);

		if ( (inputPackingType===null) || (outputPackingType===null) )
			return 0;

	} else { // not using second packing metadata byte

		switch (packing & this.COINSPARK_PACKING_INDICES_MASK) // input packing
		{
			case this.COINSPARK_PACKING_INDICES_0P_0P:
			case this.COINSPARK_PACKING_INDICES_0P_1S:
			case this.COINSPARK_PACKING_INDICES_0P_ALL:
				var inputPackingType='_0P';
				break;
		
			case this.COINSPARK_PACKING_INDICES_1S_0P:
				var inputPackingType='_1S';
				break;
	
			case this.COINSPARK_PACKING_INDICES_ALL_0P:
			case this.COINSPARK_PACKING_INDICES_ALL_1S:
			case this.COINSPARK_PACKING_INDICES_ALL_ALL:
				var inputPackingType='_ALL';
				break;
		}

		switch (packing & this.COINSPARK_PACKING_INDICES_MASK) // output packing
		{
			case this.COINSPARK_PACKING_INDICES_0P_0P:
			case this.COINSPARK_PACKING_INDICES_1S_0P:
			case this.COINSPARK_PACKING_INDICES_ALL_0P:
				var outputPackingType='_0P';
				break;
		
			case this.COINSPARK_PACKING_INDICES_0P_1S:
			case this.COINSPARK_PACKING_INDICES_ALL_1S:
				var outputPackingType='_1S';
				break;
		
			case this.COINSPARK_PACKING_INDICES_0P_ALL:
			case this.COINSPARK_PACKING_INDICES_ALL_ALL:
				var outputPackingType='_ALL';
				break;
		}
	}

//  Final stage of packing for input and output indices

	this.inputs=this.packingTypeToValues(inputPackingType, previousTransfer ? previousTransfer.inputs : null, countInputs);
	this.outputs=this.packingTypeToValues(outputPackingType, previousTransfer ? previousTransfer.outputs : null, countOutputs);

//  Read in the fields as appropriate

	var counts=this.packingToByteCounts(packing, packingExtend);
	
	var txIDPrefixBytes=counts['txIDPrefixBytes'];
	
	var read_array=[
		this.readUnsignedField(metadata, counts['blockNumBytes'], this.assetRef, 'blockNum'),
		this.readUnsignedField(metadata, counts['txOffsetBytes'], this.assetRef, 'txOffset'),
		(txIDPrefixBytes==0) ? true : (this.assetRef.txIDPrefix=CoinSparkUint8ArrayToHex(metadata.splice(0, txIDPrefixBytes)), this.assetRef.txIDPrefix.length==(2*txIDPrefixBytes)),
		this.readUnsignedField(metadata, counts['firstInputBytes'], this.inputs, 'first'),
		this.readUnsignedField(metadata, counts['countInputsBytes'], this.inputs, 'count'),
		this.readUnsignedField(metadata, counts['firstOutputBytes'], this.outputs, 'first'),
		this.readUnsignedField(metadata, counts['countOutputsBytes'], this.outputs, 'count'),
		this.readUnsignedField(metadata, counts['quantityBytes'], this, 'qtyPerOutput')
	];

	for (j=0; j<read_array.length; j++)
		if (!read_array[j])
			return 0;

//  Finish up reading in quantity

	switch (packing & this.COINSPARK_PACKING_QUANTITY_MASK)
	{
		case this.COINSPARK_PACKING_QUANTITY_1P:
			if (previousTransfer)
				this.qtyPerOutput=previousTransfer.qtyPerOutput;
			else
				this.qtyPerOutput=1;
			break;
	
		case this.COINSPARK_PACKING_QUANTITY_MAX:
			this.qtyPerOutput=COINSPARK_ASSET_QTY_MAX;
			break;
			
		case this.COINSPARK_PACKING_QUANTITY_FLOAT:
			var decodeQuantity=this.qtyPerOutput&this.COINSPARK_TRANSFER_QTY_FLOAT_MASK;
			this.qtyPerOutput=this.mantissaExponentToQty(decodeQuantity%this.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE,
				Math.floor(decodeQuantity/this.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE));
			break;
	}

//	Return bytes used

	if (!this.isValid())
		return 0;

	return startLength-metadata.length;
}

CoinSparkTransfer.prototype.toStringInner=function(headers)
{
	var buffer=headers ? "COINSPARK TRANSFER\n" : "";
	var isDefaultRoute=(this.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);
	
	if (isDefaultRoute)
		buffer+="      Default route:\n";
	
	else {
		buffer+=this.assetRef.toStringInner(false);
		buffer+="    Asset reference: "+this.assetRef.encode()+"\n";
	}
	
	if (this.inputs.count>0) {
		if (this.inputs.count>1)
			buffer+="             Inputs: "+this.inputs.first+" - "+(this.inputs.first+this.inputs.count-1)+" (count "+this.inputs.count+")";
		else
			buffer+="              Input: "+this.inputs.first;
	} else
		buffer+="             Inputs: none";
		
	buffer+=" (small endian hex: first "+this.unsignedToSmallEndianHex(this.inputs.first, 2)+" count "+
		this.unsignedToSmallEndianHex(this.inputs.count, 2)+")\n";

	if (this.outputs.count>0) {
		if ((this.outputs.count>1) && !isDefaultRoute)
			buffer+="            Outputs: "+this.outputs.first+" - "+(this.outputs.first+this.outputs.count-1)+" (count "+this.outputs.count+")";
		else
			buffer+="             Output: "+this.outputs.first;
	} else
		buffer+="            Outputs: none";
	
	buffer+=" (small endian hex: first "+this.unsignedToSmallEndianHex(this.outputs.first, 2)+
		" count "+this.unsignedToSmallEndianHex(this.outputs.count, 2)+")\n";
	
	if (!isDefaultRoute) {
		buffer+="     Qty per output: "+this.qtyPerOutput+" (small endian hex "+this.unsignedToSmallEndianHex(this.qtyPerOutput, 8);
		
		var result=this.qtyToMantissaExponent(this.qtyPerOutput, 0,
			this.COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX, this.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX);
		
		if (result.qty==this.qtyPerOutput) {
			var encodeQuantity=(result.exponent*this.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+result.mantissa)&this.COINSPARK_TRANSFER_QTY_FLOAT_MASK;		
			buffer+=", as float "+this.unsignedToSmallEndianHex(encodeQuantity, this.COINSPARK_TRANSFER_QTY_FLOAT_LENGTH);
		}

		buffer+=")\n";
	}
	
	if (headers)
		buffer+="END COINSPARK TRANSFER\n\n";
	
	return buffer;
}

CoinSparkTransfer.prototype.getPackingOptions=function(previousRange, range, countInputsOutputs)
{
	var packingOptions=[];
	
	var firstZero=(range.first==0);
	var firstByte=(range.first<=COINSPARK_UNSIGNED_BYTE_MAX);
	var first2Bytes=(range.first<=COINSPARK_UNSIGNED_2_BYTES_MAX);
	var countOne=(range.count==1);
	var countByte=(range.count<=COINSPARK_UNSIGNED_BYTE_MAX);
	
	if (previousRange) {
		packingOptions['_0P']=(range.first==previousRange.first) && (range.count==previousRange.count);
		packingOptions['_1S']=(range.first==(previousRange.first+previousRange.count)) && countOne;
		
	} else {
		packingOptions['_0P']=firstZero && countOne;
		packingOptions['_1S']=(range.first==1) && countOne;
	}
	
	packingOptions['_BYTE']=firstByte && countOne;
	packingOptions['_2_BYTES']=first2Bytes && countOne;
	packingOptions['_1_1_BYTES']=firstByte && countByte;
	packingOptions['_2_1_BYTES']=first2Bytes && countByte;
	packingOptions['_2_2_BYTES']=first2Bytes && (range.count<=COINSPARK_UNSIGNED_2_BYTES_MAX);
	packingOptions['_ALL']=firstZero && (range.count>=countInputsOutputs);
	
	return packingOptions;
}

CoinSparkTransfer.prototype.packingTypeToValues=function(packingType, previousRange, countInputOutputs)
{
	var range=new CoinSparkInOutRange();
	
	switch (packingType)
	{
		case '_0P':
			if (previousRange) {
				range.first=previousRange.first;
				range.count=previousRange.count;
			} else {
				range.first=0;
				range.count=1;
			}
			break;
	
		case '_1S':
			if (previousRange)
				range.first=previousRange.first+previousRange.count;
			else
				range.first=1;
	
			range.count=1;
			break;
	
		case '_BYTE':
		case '_2_BYTES':
			range.count=1;
			break;
	
		case '_ALL':
			range.first=0;
			range.count=countInputOutputs;
			break;
	}
	
	return range;
}

CoinSparkTransfer.prototype.packingToByteCounts=function(packing, packingExtend)
{

//  Set default values for bytes for all fields to zero

	var counts={
		'blockNumBytes': 0,
		'txOffsetBytes': 0,
		'txIDPrefixBytes': 0,
		
		'firstInputBytes': 0,
		'countInputsBytes': 0,
		'firstOutputBytes': 0,
		'countOutputsBytes': 0,
		
		'quantityBytes': 0
	};
	
//	Packing for genesis reference

	switch (packing & this.COINSPARK_PACKING_GENESIS_MASK)
	{
		case this.COINSPARK_PACKING_GENESIS_3_3_BYTES:
			counts['blockNumBytes']=3;
			counts['txOffsetBytes']=3;
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
			break;
	
		case this.COINSPARK_PACKING_GENESIS_3_4_BYTES:
			counts['blockNumBytes']=3;
			counts['txOffsetBytes']=4;
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
			break;
	
		case this.COINSPARK_PACKING_GENESIS_4_4_BYTES:
			counts['blockNumBytes']=4;
			counts['txOffsetBytes']=4;
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
			break;
	}

//  Packing for input and output indices (relevant for extended indices only)

	if ((packing & this.COINSPARK_PACKING_INDICES_MASK) == this.COINSPARK_PACKING_INDICES_EXTEND) {

	//  Input indices

		switch ((packingExtend >> this.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & this.COINSPARK_PACKING_EXTEND_MASK)
		{
			case this.COINSPARK_PACKING_EXTEND_BYTE:
				counts['firstInputBytes']=1;
				break;
		
			case this.COINSPARK_PACKING_EXTEND_2_BYTES:
				counts['firstInputBytes']=2;
				break;
		
			case this.COINSPARK_PACKING_EXTEND_1_1_BYTES:
				counts['firstInputBytes']=1;
				counts['countInputsBytes']=1;
				break;
		
			case this.COINSPARK_PACKING_EXTEND_2_1_BYTES:
				counts['firstInputBytes']=2;
				counts['countInputsBytes']=1;
				break;
		
			case this.COINSPARK_PACKING_EXTEND_2_2_BYTES:
				counts['firstInputBytes']=2;
				counts['countInputsBytes']=2;
				break;
		}

	//  Output indices

		switch ((packingExtend >> this.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & this.COINSPARK_PACKING_EXTEND_MASK)
		{
			case this.COINSPARK_PACKING_EXTEND_BYTE:
				counts['firstOutputBytes']=1;
				break;
		
			case this.COINSPARK_PACKING_EXTEND_2_BYTES:
				counts['firstOutputBytes']=2;
				break;
		
			case this.COINSPARK_PACKING_EXTEND_1_1_BYTES:
				counts['firstOutputBytes']=1;
				counts['countOutputsBytes']=1;
				break;
		
			case this.COINSPARK_PACKING_EXTEND_2_1_BYTES:
				counts['firstOutputBytes']=2;
				counts['countOutputsBytes']=1;
				break;
		
			case this.COINSPARK_PACKING_EXTEND_2_2_BYTES:
				counts['firstOutputBytes']=2;
				counts['countOutputsBytes']=2;
				break;
		}

	}

//  Packing for quantity

	switch (packing & this.COINSPARK_PACKING_QUANTITY_MASK)
	{
		case this.COINSPARK_PACKING_QUANTITY_1_BYTE:
			counts['quantityBytes']=1;
			break;
	
		case this.COINSPARK_PACKING_QUANTITY_2_BYTES:
			counts['quantityBytes']=2;
			break;
	
		case this.COINSPARK_PACKING_QUANTITY_3_BYTES:
			counts['quantityBytes']=3;
			break;
	
		case this.COINSPARK_PACKING_QUANTITY_4_BYTES:
			counts['quantityBytes']=4;
			break;
	
		case this.COINSPARK_PACKING_QUANTITY_6_BYTES:
			counts['quantityBytes']=6;
			break;
	
		case this.COINSPARK_PACKING_QUANTITY_FLOAT:
			counts['quantityBytes']=this.COINSPARK_TRANSFER_QTY_FLOAT_LENGTH;
			break;
	}

//	Return the resulting array
	
	return counts;
}

CoinSparkTransfer.prototype.getPackingExtendMap=function()
{
	 return {
		'_0P':this.COINSPARK_PACKING_EXTEND_0P,
		'_1S':this.COINSPARK_PACKING_EXTEND_1S,
		'_ALL':this.COINSPARK_PACKING_EXTEND_ALL,
		'_BYTE':this.COINSPARK_PACKING_EXTEND_BYTE,
		'_2_BYTES':this.COINSPARK_PACKING_EXTEND_2_BYTES,
		'_1_1_BYTES':this.COINSPARK_PACKING_EXTEND_1_1_BYTES,
		'_2_1_BYTES':this.COINSPARK_PACKING_EXTEND_2_1_BYTES,
		'_2_2_BYTES':this.COINSPARK_PACKING_EXTEND_2_2_BYTES
	}; // in order of preference
}

CoinSparkTransfer.prototype.encodePackingExtend=function(packingOptions)
{
	var packingExtendMap=this.getPackingExtendMap();
	
	for (var packingType in packingExtendMap)
		if (packingOptions[packingType])
			return packingExtendMap[packingType];
			
	return null;
}

CoinSparkTransfer.prototype.decodePackingExtend=function(packingExtend)
{
	var packingExtendMap=this.getPackingExtendMap();
	
	for (var packingType in packingExtendMap)
		if (packingExtend==packingExtendMap[packingType])
			return packingType;
			
	return null;
}

CoinSparkTransfer.prototype.writeUnsignedField=function(bytes, source)
{
	return (bytes>0) ? this.writeSmallEndianUnsigned(source, bytes) : []; // will return null on failure
}

CoinSparkTransfer.prototype.readUnsignedField=function(metadata, bytes, object, property)
{
	if (bytes>0) {
		var value=this.shiftReadSmallEndianUnsigned(metadata.splice(0, bytes), bytes);
		if (value===null)
			return false;
		
		object[property]=value;
	}
	
	return true;
}


//	CoinSparkTransferList class for managing list of asset transfer metadata

function CoinSparkTransferList()
{
	this.clear();
}

CoinSparkTransferList.prototype=new CoinSparkBase();

CoinSparkTransferList.prototype.clear=function()
{
	this.transfers=[];
}

CoinSparkTransferList.prototype.toString=function()
{
	var buffer="COINSPARK TRANSFERS\n";

	for (var transferIndex=0; transferIndex<this.transfers.length; transferIndex++) {
		if (transferIndex>0)
			buffer+="\n";
		
		buffer+=this.transfers[transferIndex].toStringInner(false);
	}

	buffer+="END COINSPARK TRANSFERS\n\n";

	return buffer;
}

CoinSparkTransferList.prototype.isValid=function()
{
	if (!(this.transfers instanceof Array))
		return false;
	
	for (var transferIndex=0; transferIndex<this.transfers.length; transferIndex++)
		if (!this.transfers[transferIndex].isValid())
			return false;
			
	return true;
}

CoinSparkTransferList.prototype.match=function(otherTransfers, strict)
{
	var countTransfers=this.transfers.length;
	if (countTransfers!=otherTransfers.transfers.length)
		return false;
	
	if (strict) {
		for (var transferIndex=0; transferIndex<countTransfers; transferIndex++)
			if (!this.transfers[transferIndex].match(otherTransfers.transfers[transferIndex]))
				return false;
	
	} else {
		var thisOrdering=this.groupOrdering();
		var otherOrdering=otherTransfers.groupOrdering();
		
		for (var transferIndex=0; transferIndex<countTransfers; transferIndex++)
			if (!this.transfers[thisOrdering[transferIndex]].match(otherTransfers.transfers[otherOrdering[transferIndex]]))
				return false;
	}

	return true;
}

CoinSparkTransferList.prototype.encode=function(countInputs, countOutputs, metadataMaxLen)
{

//  4-character identifier

	var metadata=CoinSparkASCIIToUint8Array(COINSPARK_METADATA_IDENTIFIER+COINSPARK_TRANSFERS_PREFIX);

//	Encode each transfer, grouping by asset reference, but preserving original order otherwise

	var ordering=this.groupOrdering();
	
	var countTransfers=this.transfers.length;
	var previousTransfer=null;
	
	for (transferIndex=0; transferIndex<countTransfers; transferIndex++) {
		var thisTransfer=this.transfers[ordering[transferIndex]];
		
		var array=thisTransfer.encode(previousTransfer, metadataMaxLen-metadata.length, countInputs, countOutputs);
		if (!array)
			return null;
			
		metadata=metadata.concat(array);
		previousTransfer=thisTransfer;
	}

//	Extra length check (even though thisTransfer.encode() should be sufficient)

	if (metadata.length>metadataMaxLen)
		return null;
		
//	Return what we created

	return metadata;			
}

CoinSparkTransferList.prototype.decode=function(metadata, countInputs, countOutputs)
{
	metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_TRANSFERS_PREFIX);
	if (!metadata)
		return 0;

//	Iterate over list

	this.transfers=[];
	var previousTransfer=null;

	while (metadata.length>0) {
		var transfer=new CoinSparkTransfer();
		var transferBytesUsed=transfer.decode(metadata, previousTransfer, countInputs, countOutputs);
		
		if (transferBytesUsed>0) {
			this.transfers[this.transfers.length]=transfer;
			metadata.splice(0, transferBytesUsed);
			previousTransfer=transfer;
		
		} else
			return 0; // something was invalid
	}

//	Return count

	return this.transfers.length;
}

CoinSparkTransferList.prototype.calcMinFee=function(countInputs, outputsSatoshis, outputsRegular)
{
	var countOutputs=outputsSatoshis.length;
	if (countOutputs!=outputsRegular.length)
		return COINSPARK_SATOSHI_QTY_MAX; // these two arrays must be the same size
	
	var transfersToCover=0;
	
	for (var index=0; index<this.transfers.length; index++) {
		var transfer=this.transfers[index];
		
		if (
			(transfer.assetRef.blockNum != COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) && // don't count default routes
			(transfer.inputs.count>0) &&
			(transfer.inputs.first<countInputs) // only count if at least one valid input index
		) {
			var outputIndex=Math.max(transfer.outputs.first, 0);
			var lastOutputIndex=Math.min(transfer.outputs.first+transfer.outputs.count, countOutputs)-1;
	
			for (; outputIndex<=lastOutputIndex; outputIndex++)
				if (outputsRegular[outputIndex])
					transfersToCover++;
		}
	}
	
	return transfersToCover*this.getMinFeeBasis(outputsSatoshis, outputsRegular);
}

CoinSparkTransferList.prototype.apply=function(assetRef, genesis, _inputBalances, outputsRegular)
{
	inputBalances=_inputBalances.slice(0); // copy since we will modify it
	for (var inputIndex=0; inputIndex<inputBalances.length; inputIndex++)
		inputBalances[inputIndex]=parseInt(inputBalances[inputIndex]); // prevents + performing concatenation

//	Zero output quantities and get counts
	
	var countInputs=inputBalances.length;
	var countOutputs=outputsRegular.length;
	var outputBalances=CoinSparkArrayFill(0, countOutputs, 0);
	
//  Perform explicit transfers (i.e. not default routes)
	
	for (var transferIndex=0; transferIndex<this.transfers.length; transferIndex++) {
		var transfer=this.transfers[transferIndex];
		
		if (assetRef.match(transfer.assetRef)) {
			var inputIndex=Math.max(transfer.inputs.first, 0);
			var outputIndex=Math.max(transfer.outputs.first, 0);
	
			var lastInputIndex=Math.min(inputIndex+transfer.inputs.count, countInputs)-1;
			var lastOutputIndex=Math.min(outputIndex+transfer.outputs.count, countOutputs)-1;
	
			for (; outputIndex<=lastOutputIndex; outputIndex++)
				if (outputsRegular[outputIndex]) {
					var transferRemaining=transfer.qtyPerOutput;
			
					while (inputIndex<=lastInputIndex) {
						var transferQuantity=Math.min(transferRemaining, inputBalances[inputIndex]);
				
						if (transferQuantity>0) { // skip all this if nothing is to be transferred (branch not really necessary)
							inputBalances[inputIndex]-=transferQuantity;
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
	
//	Apply payment charges to all quantities not routed by default

	for (outputIndex=0; outputIndex<countOutputs; outputIndex++)
		if (outputsRegular[outputIndex])
			outputBalances[outputIndex]=genesis.calcNet(outputBalances[outputIndex]);
			
//	Send remaining quantities to default outputs

	inputDefaultOutput=this.getDefaultRouteMap(countInputs, outputsRegular);
	
	for (var inputIndex=0; inputIndex<inputDefaultOutput.length; inputIndex++) {
		var outputIndex=inputDefaultOutput[inputIndex];
	
		if (outputIndex!==null)
			outputBalances[outputIndex]+=inputBalances[inputIndex];
	}
			
//	Return the result

	return outputBalances;
}

CoinSparkTransferList.prototype.applyNone=function(assetRef, genesis, inputBalances, outputsRegular)
{
	var countOutputs=outputsRegular.length;
	var outputBalances=CoinSparkArrayFill(0, countOutputs, 0);

	var outputIndex=this.getLastRegularOutput(outputsRegular);
	if (outputIndex!==null)
		for (var inputIndex=0; inputIndex<inputBalances.length; inputIndex++)
			outputBalances[outputIndex]+=parseInt(inputBalances[inputIndex]); // to prevent concatenation
		
	return outputBalances;
}

CoinSparkTransferList.prototype.defaultOutputs=function(countInputs, outputsRegular)
{
	var outputsDefault=CoinSparkArrayFill(0, outputsRegular.length, false);
	
	var inputDefaultOutput=this.getDefaultRouteMap(countInputs, outputsRegular);
	
	for (var inputIndex=0; inputIndex<inputDefaultOutput.length; inputIndex++) {
		var outputIndex=inputDefaultOutput[inputIndex];
		
		if (outputIndex!==null)
			outputsDefault[outputIndex]=true;
	}
			
	return outputsDefault;
}

CoinSparkTransferList.prototype.groupOrdering=function()
{
	var countTransfers=this.transfers.length;
	var transferUsed=CoinSparkArrayFill(0, countTransfers, false);
	var ordering=[];

	for (orderIndex=0; orderIndex<countTransfers; orderIndex++) {
		var bestTransferScore=0;
		var bestTransferIndex=-1;
		
		for (transferIndex=0; transferIndex<countTransfers; transferIndex++) {
			var transfer=this.transfers[transferIndex];
			
			if (!transferUsed[transferIndex]) {
				if (transfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
					var transferScore=3; // top priority to default routes, which must be first in the encoded list
				else if ((orderIndex>0) && transfer.assetRef.match(this.transfers[ordering[orderIndex-1]].assetRef))
					var transferScore=2; // then next best is one which has same asset reference as previous
				else
					var transferScore=1; // otherwise any will do
					
				if (transferScore>bestTransferScore) { // if it's clearly the best, take it
					bestTransferScore=transferScore;
					bestTransferIndex=transferIndex;
					
				} else if (transferScore==bestTransferScore) // otherwise give priority to "lower" asset references
					if (transfer.assetRef.compare(this.transfers[bestTransferIndex].assetRef)<0)
						bestTransferIndex=transferIndex;
			}
		}
			
		ordering[orderIndex]=bestTransferIndex;
		transferUsed[bestTransferIndex]=true;
	}
	
	return ordering;
}

CoinSparkTransferList.prototype.getDefaultRouteMap=function(countInputs, outputsRegular)
{
	var countOutputs=outputsRegular.length;
	
//  Default to last output for all inputs

	var inputDefaultOutput=CoinSparkArrayFill(0, countInputs, this.getLastRegularOutput(outputsRegular));
	
//  Apply any default route transfers in reverse order (since early ones take precedence)

	for (var transferIndex=this.transfers.length-1; transferIndex>=0; transferIndex--) {
		var transfer=this.transfers[transferIndex];

		if (transfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) {
			var outputIndex=transfer.outputs.first;
			
			if ((outputIndex>=0) && (outputIndex<countOutputs)) {
				var inputIndex=Math.max(transfer.inputs.first, 0);
				var lastInputIndex=Math.min(inputIndex+transfer.inputs.count, countInputs)-1;
				
				for (; inputIndex<=lastInputIndex; inputIndex++)
					inputDefaultOutput[inputIndex]=outputIndex;
			}
		}
	}
		
//	Return the result
		
	return inputDefaultOutput;
}


//	CoinSparkPaymentRef class for managing payment references

function CoinSparkPaymentRef()
{
	this.clear();
}

CoinSparkPaymentRef.prototype=new CoinSparkBase();

CoinSparkPaymentRef.prototype.clear=function()
{
	this.ref=0;
}

CoinSparkPaymentRef.prototype.toString=function()
{
	var buffer="COINSPARK PAYMENT REFERENCE\n";
	buffer+=this.ref+" (small endian hex "+this.unsignedToSmallEndianHex(this.ref, 8)+")\n";
	buffer+="END COINSPARK PAYMENT REFERENCE\n\n";
	
	return buffer;
}

CoinSparkPaymentRef.prototype.isValid=function()
{
	return (this.ref>=0) && (this.ref<=COINSPARK_PAYMENT_REF_MAX) && this.isInteger(this.ref);
}

CoinSparkPaymentRef.prototype.match=function(otherPaymentRef)
{
	return this.ref==otherPaymentRef.ref;
}

CoinSparkPaymentRef.prototype.randomize=function()
{
	this.ref=0;
	
	for (var bitsRemaining=COINSPARK_PAYMENT_REF_MAX; bitsRemaining>0; bitsRemaining=Math.floor(bitsRemaining/8192)) {
		this.ref*=8192;
		this.ref+=Math.floor(Math.random()*8192);
	}
	
	return this.ref=Math.round(this.ref%(1+COINSPARK_PAYMENT_REF_MAX));	
}

CoinSparkPaymentRef.prototype.encode=function(metadataMaxLen)
{
	if (!this.isValid())
		return null;
		
//	4-character identifier
		
	var metadata=CoinSparkASCIIToUint8Array(COINSPARK_METADATA_IDENTIFIER+COINSPARK_PAYMENTREF_PREFIX);

//	The payment reference
	
	var bytes=0;
	for (var paymentLeft=this.ref; paymentLeft>0; paymentLeft=Math.floor(paymentLeft/256))
		bytes++;
		
	metadata=metadata.concat(this.writeSmallEndianUnsigned(this.ref, bytes));
	
//	Check the total length is within the specified limit

	if (metadata.length>metadataMaxLen)
		return null;
		
//	Return what we created

	return metadata;
}

CoinSparkPaymentRef.prototype.decode=function(metadata)
{
	metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_PAYMENTREF_PREFIX);
	if (!metadata)
		return false;
		
//	The payment reference

	finalMetadataLen=metadata.length;
	if (finalMetadataLen>8)
		return false;

	this.ref=this.readSmallEndianUnsigned(metadata, finalMetadataLen);
		
//	Return validity

	return this.isValid();			
}


//	Class used internally for input or output ranges	

function CoinSparkInOutRange()
{
	this.clear();
}

CoinSparkInOutRange.prototype=new CoinSparkBase();

CoinSparkInOutRange.prototype.clear=function()
{
	this.first=0;
	this.count=0;
}

CoinSparkInOutRange.prototype.isValid=function()
{
	if ( (this.first<0) || (this.first>COINSPARK_IO_INDEX_MAX) || !this.isInteger(this.first) )
		return false;
		
	if ( (this.count<0) || (this.count>COINSPARK_IO_INDEX_MAX) || !this.isInteger(this.count) )
		return false;
	
	return true;
}

CoinSparkInOutRange.prototype.match=function(otherInOutRange)
{
	return (this.first==otherInOutRange.first) && (this.count==otherInOutRange.count);
}


//	Base class implementing utility functions used internally

function CoinSparkBase()
{
}

CoinSparkBase.prototype.isInteger=function(value)
{
	var parsed=parseFloat(value);
	
	return (typeof parsed == 'number') && (parsed == parseInt(value)); // allows numerical strings
}

CoinSparkBase.prototype.isBoolean=function(value)
{
	return (value===true) || (value===false) || (this.isInteger(value) && ((value==0) || (value==1)));
		// allows anything that could reasonably be used to represent a true or false value
}

CoinSparkBase.prototype.isString=function(value)
{
	return (typeof value == 'string') || (typeof value == 'number'); // also allows integers and floats
}

CoinSparkBase.prototype.isUInt8Array=function(value)
{
	if (!(value instanceof Array))
		return false;
		
	for (var index=0; index<value.length; index++) {
		var element=value[index];
		if ((element<0) || (element>255) || !this.isInteger(element))
			return false;
	}
	
	return true;
}

CoinSparkBase.prototype.writeSmallEndianUnsigned=function(value, bytes)
{
	if (value<0)
		return null; // does not support negative values
		
	var array=[];
	
	for (var byte=0; byte<bytes; byte++) {
		array.push(Math.round(value%256));
		value=Math.floor(value/256);
	}
	
	return value ? null : array;
}

CoinSparkBase.prototype.readSmallEndianUnsigned=function(array, bytes)
{
	if (array.length<bytes)
		return null;

	var value=0;

	for (var byte=bytes-1; byte>=0; byte--) {
		value*=256;
		value+=array[byte];
	}

	return value;
}

CoinSparkBase.prototype.shiftReadSmallEndianUnsigned=function(array, bytes)
{
	return this.readSmallEndianUnsigned(array.splice(0, bytes), bytes);
}

CoinSparkBase.prototype.unsignedToSmallEndianHex=function(value, bytes)
{
	var string='';

	for (var byte=0; byte<bytes; byte++) {
		digits='00'+Math.round(value%256).toString(16).toUpperCase();
		string+=digits.substr(digits.length-2, 2);
		value=Math.floor(value/256);
	}
	
	return string;
}

CoinSparkBase.prototype.base58ToInteger=function(base58Character)
{
	var integer=COINSPARK_INTEGER_TO_BASE_58.indexOf(base58Character);
	
	return (integer<0) ? null : integer;
}

CoinSparkBase.prototype.mantissaExponentToQty=function(mantissa, exponent)
{
	var quantity=mantissa;
	
	for (; exponent>0; exponent--)
		quantity*=10;
		
	return quantity;
}

CoinSparkBase.prototype.qtyToMantissaExponent=function(quantity, rounding, mantissaMax, exponentMax)
{
	if (rounding<0)
		var roundOffset=0;
	else if (rounding>0)
		var roundOffset=9;
	else
		var roundOffset=4;
	
	var exponent=0;
	
	while (quantity>mantissaMax) {
		quantity=Math.floor((quantity+roundOffset)/10);
		exponent++;
	}
	
	var exponent=Math.min(exponent, exponentMax);

	return {
		'mantissa': quantity,
		'exponent': exponent,
		'qty': this.mantissaExponentToQty(quantity, exponent)
	};
}

CoinSparkBase.prototype.getMinFeeBasis=function(outputsSatoshis, outputsRegular)
{
	var smallestOutputSatoshis=COINSPARK_SATOSHI_QTY_MAX;

	if (outputsSatoshis.length==outputsRegular.length) // if arrays different size, we can't use them
		for (var outputIndex=0; outputIndex<outputsSatoshis.length; outputIndex++)
			if (outputsRegular[outputIndex])
				smallestOutputSatoshis=Math.min(smallestOutputSatoshis, outputsSatoshis[outputIndex]);

	return Math.min(COINSPARK_FEE_BASIS_MAX_SATOSHIS, smallestOutputSatoshis);
}

CoinSparkBase.prototype.getLastRegularOutput=function(outputsRegular)
{
	var lastRegularOutput=null;
	
	for (var outputIndex=0; outputIndex<outputsRegular.length; outputIndex++)
		if (outputsRegular[outputIndex])
			lastRegularOutput=outputIndex;
			
	return lastRegularOutput;
}

CoinSparkBase.prototype.countNonLastRegularOutputs=function(outputsRegular)
{
	var countRegularOutputs=0;
	
	for (var outputIndex=0; outputIndex<outputsRegular.length; outputIndex++)
		if (outputsRegular[outputIndex])
			countRegularOutputs++;
			
	return Math.max(countRegularOutputs-1, 0);
}

CoinSparkBase.prototype.shrinkLowerDomainName=function(domainName)
{
	if (!domainName.length)
		return null;
	
	domainName=domainName.toLowerCase();

//	Search for prefixes

	var bestPrefixLen=-1;
	for (var prefixIndex=0; prefixIndex<COINSPARK_DOMAIN_NAME_PREFIXES.length; prefixIndex++) {
		var prefix=COINSPARK_DOMAIN_NAME_PREFIXES[prefixIndex];
		var prefixLen=prefix.length;
	
		if ( (prefixLen>bestPrefixLen) && (domainName.substr(0, prefixLen)==prefix) ) {
			var bestPrefixIndex=prefixIndex;
			bestPrefixLen=prefixLen;
		}
	}

	domainName=domainName.substr(bestPrefixLen);
	domainNameLen=domainName.length;

//	Search for suffixes

	var bestSuffixLen=-1;
	for (var suffixIndex=0; suffixIndex<COINSPARK_DOMAIN_NAME_SUFFIXES.length; suffixIndex++) {
		var suffix=COINSPARK_DOMAIN_NAME_SUFFIXES[suffixIndex];
		var suffixLen=suffix.length;
	
		if ( (suffixLen>bestSuffixLen) && (domainName.substr(domainNameLen-suffixLen)==suffix) ) {
			var bestSuffixIndex=suffixIndex;
			bestSuffixLen=suffixLen;
		}
	}

	domainName=domainName.substr(0, domainNameLen-bestSuffixLen);

//	Output and return

	var packing=((bestPrefixIndex<<COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT)&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)|
		(bestSuffixIndex&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK);

	return {
		'domainName': domainName,
		'packing': packing
	};
}

CoinSparkBase.prototype.expandDomainName=function(domainName, packing)
{

//	Prefix

	var prefixIndex=(packing&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)>>COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT;
	var prefix=COINSPARK_DOMAIN_NAME_PREFIXES[prefixIndex];
	if (typeof prefix == 'undefined')
		return null;

//	Suffix

	var suffixIndex=packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK;
	var suffix=COINSPARK_DOMAIN_NAME_SUFFIXES[suffixIndex];
	if (typeof suffix == 'undefined')
		return null;

	return prefix+domainName+suffix;
}

CoinSparkBase.prototype.readIPv4Address=function(domainName)
{
	if (domainName.search(/[^0-9\.]/)>=0)
		return null;
	
	var octets=domainName.split('.');
	if (octets.length!=4)
		return null;
		
	for (var octetIndex=0; octetIndex<4; octetIndex++)
		if ( (octets[octetIndex].length==0) || (octets[octetIndex]>255) )
			return null;
			
	return octets;
}

CoinSparkBase.prototype.encodeDomainPathTriplets=function(string)
{
	var stringLen=string.length;
	var metadata=[];

	for (var stringPos=0; stringPos<stringLen; stringPos++) {
		var encodeValue=COINSPARK_DOMAIN_NAME_CHARS.indexOf(string.charAt(stringPos));
		if (encodeValue<0)
			return null;

		switch (stringPos%3) {
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
			var array=this.writeSmallEndianUnsigned(stringTriplet, 2);
			if (!array)
				return null;

			metadata=metadata.concat(array);
		}
	}
	
	return metadata;
}

CoinSparkBase.prototype.shiftDecodeDomainPathTriplets=function(metadata, parts)
{
	var string='';
	var stringPos=0;
	
	while (parts>0) {

		if ((stringPos%3)==0) {
			var stringTriplet=this.shiftReadSmallEndianUnsigned(metadata, 2);
			if (stringTriplet===null)
				return null;

			if (stringTriplet>=(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE))
				return null; // invalid value
		}

		switch (stringPos%3)
		{
			case 0:
				var decodeValue=stringTriplet%COINSPARK_DOMAIN_PATH_ENCODE_BASE;
				break;
		
			case 1:
				var decodeValue=Math.floor(stringTriplet/COINSPARK_DOMAIN_PATH_ENCODE_BASE)%COINSPARK_DOMAIN_PATH_ENCODE_BASE;
				break;
		
			case 2:
				var decodeValue=Math.floor(stringTriplet/(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE));
				break;
		}

		var decodeChar=COINSPARK_DOMAIN_NAME_CHARS.charAt(decodeValue);
		string+=decodeChar;
		stringPos++;

		if ((decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR) || (decodeChar==COINSPARK_DOMAIN_PATH_FALSE_END_CHAR))
			parts--;
	}

	return string;
}

CoinSparkBase.prototype.encodeDomainAndOrPath=function(domainName, useHttps, pagePath, usePrefix)
{
	var metadata=[];
	var encodeString='';
	
//  Domain name

	if (domainName!=null) {
		var octets=this.readIPv4Address(domainName);

		if (octets!=null) {
			metadata=metadata.concat([
				COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4+(useHttps ? COINSPARK_DOMAIN_PACKING_IPv4_HTTPS : 0),
				parseInt(octets[0], 10),
				parseInt(octets[1], 10),
				parseInt(octets[2], 10),
				parseInt(octets[3], 10)
			]);

		} else {
			var result=this.shrinkLowerDomainName(domainName);
			encodeString+=result.domainName;
			encodeString+=useHttps ? COINSPARK_DOMAIN_PATH_TRUE_END_CHAR : COINSPARK_DOMAIN_PATH_FALSE_END_CHAR;
			
			metadata.push(result.packing);
		}
	}
	
//	Page path

	if (pagePath!=null) {
		encodeString+=pagePath;
		encodeString+=usePrefix ? COINSPARK_DOMAIN_PATH_TRUE_END_CHAR : COINSPARK_DOMAIN_PATH_FALSE_END_CHAR;
	}
	
//	Encode whatever is required as triplets

	if (encodeString.length) {
		var array=this.encodeDomainPathTriplets(encodeString);
		if (!array)
			return null;
			
		metadata=metadata.concat(array);
	}
	
	return metadata;			
}

CoinSparkBase.prototype.shiftDecodeDomainAndOrPath=function(metadata, doDomainName, doPagePath)
{
	var result=[];
	var metadataParts=0;

//	Domain name

	if (doDomainName) {
	
	//	Get packing byte
	
		var packing=metadata.shift();
		if (typeof packing == 'undefined')
			return null;
	
	//	Extract IP address if present
		
		var isIpAddress=((packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK)==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4);
		
		if (isIpAddress) {
			result['useHttps']=(packing&COINSPARK_DOMAIN_PACKING_IPv4_HTTPS) ? true : false;
	
			var octets=metadata.splice(0, 4);
			if (octets.length!=4)
				return null;
		
			result['domainName']=octets[0]+'.'+octets[1]+'.'+octets[2]+'.'+octets[3];

		} else
			metadataParts++;
	}
	
//	Convert remaining metadata to string

	if (doPagePath)
		metadataParts++;

	if (metadataParts>0) {
		var decodeString=this.shiftDecodeDomainPathTriplets(metadata, metadataParts);
		if (decodeString==null)
			return null;

	//  Extract domain name if IP address was not present
		
		var endRegExp=new RegExp('[\\'+COINSPARK_DOMAIN_PATH_FALSE_END_CHAR+'\\'+COINSPARK_DOMAIN_PATH_TRUE_END_CHAR+']');
		
		if (doDomainName && !isIpAddress) {
			var endCharPos=decodeString.search(endRegExp);
			if (endCharPos<0)
				return null; // should never happen
			
			result['domainName']=this.expandDomainName(decodeString.substr(0, endCharPos), packing);
			if (result['domainName']==null)
				return null;
				
			result['useHttps']=(decodeString.charAt(endCharPos)==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR);

			decodeString=decodeString.substr(endCharPos+1);
		}

	//  Extract page path

		if (doPagePath) {
			var endCharPos=decodeString.search(endRegExp);
			if (endCharPos<0)
				return null; // should never happen
				
			result['pagePath']=decodeString.substr(0, endCharPos);
			result['usePrefix']=(decodeString.charAt(endCharPos)==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR);
			decodeString=decodeString.substr(endCharPos+1);
		}
	}

//  Finish and return

	return result;
}


//	Other functions used internally
	
function CoinSparkGetRawScript(scriptPubKey, scriptIsHex)
{
	if (scriptIsHex) {
		if ( (scriptPubKey.length%2) || (scriptPubKey.search(/[^0-9A-Fa-f]/)>=0) )
			return null;
		else
			return CoinSparkHexToUint8Array(scriptPubKey);
	} else
		return scriptPubKey;
}

function CoinSparkLocateMetadataRange(metadata, desiredPrefix)
{
	var metadataLen=metadata.length;

	if (metadataLen<(COINSPARK_METADATA_IDENTIFIER_LEN+1)) // check for 4 bytes at least
		return null; 
	
	if (CoinSparkUint8ArrayToASCII(metadata.slice(0, COINSPARK_METADATA_IDENTIFIER_LEN))!=COINSPARK_METADATA_IDENTIFIER) // check it starts 'SPK'
		return null; 
	
	var position=COINSPARK_METADATA_IDENTIFIER_LEN; // start after 'SPK'

	while (position<metadataLen) {
		var foundPrefixOrd=metadata[position]; // read the next prefix
		var foundPrefix=String.fromCharCode(foundPrefixOrd);
		position++;
	
		if ( (desiredPrefix!=null) ? (foundPrefix==desiredPrefix) : (foundPrefixOrd>COINSPARK_LENGTH_PREFIX_MAX) )
			// it's our data from here to the end (if desiredPrefix is null, it matches the last one whichever it is)
			return metadata.slice(position);
	
		if (foundPrefixOrd>COINSPARK_LENGTH_PREFIX_MAX) // it's some other type of data from here to end
			return null;
		
		// if we get here it means we found a length byte
	
		if ((position+foundPrefixOrd)>metadataLen) // something went wrong - length indicated is longer than that available
			return null;
		
		if (position>=metadataLen) // something went wrong - that was the end of the input data
			return null; 
		
		if (String.fromCharCode(metadata[position])==desiredPrefix) // it's the length of our part
			return metadata.slice(position+1, position+foundPrefixOrd);
		else
			position+=foundPrefixOrd; // skip over this many bytes
	}

	return null;
}

function CoinSparkArrayFill(start, num, value)
{
	var array=[];
	
	for (index=start; index<(start+num); index++)
		array[index]=value;
	
	return array;
}

function CoinSparkHexToUint8Array(hex)
{
	var array=[];

	for (var bytePair=0; bytePair<hex.length; bytePair+=2)
		array.push(parseInt(hex.substr(bytePair, 2), 16));

	return array;
}

function CoinSparkUint8ArrayToHex(uint8Array)
{
	var buffer='';
	
	for (var byte=0; byte<uint8Array.length; byte++) {
		var hex='00'+uint8Array[byte].toString(16);
		buffer+=hex.substr(hex.length-2).toUpperCase();
	}
	
	return buffer;
}

function CoinSparkASCIIToUint8Array(string)
{
	var array=[];
	
	for (var byte=0; byte<string.length; byte++)
		array.push(string.charCodeAt(byte));
		
	return array;
}

function CoinSparkUint8ArrayToASCII(array)
{
	var string='';
	
	for (var byte=0; byte<array.length; byte++)
		string+=String.fromCharCode(array[byte]);
	
	return string;
}

function CoinSparkStringToUint8ArrayUTF8(string)
{
	return CoinSparkASCIIToUint8Array(unescape(encodeURIComponent(string)));
}

function CoinSparkUint8ArrayUTF8ToString(array)
{
	return decodeURIComponent(escape(CoinSparkUint8ArrayToASCII(array)));
}

function CoinSparkUint8ArraySHA256(array)
{
	return sjcl.codec.bytes.fromBits(sjcl.hash.sha256.hash(sjcl.codec.bytes.toBits(array)));
}


//	Code below is adapted from SJCL, Stanford Javascript Crypto Library.

/*
Copyright 2009-2010 Emily Stark, Mike Hamburg, Dan Boneh.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation
are those of the authors and should not be interpreted as representing
official policies, either expressed or implied, of the authors.
*/

/******************** sjcl.js ********************/

var sjcl = {
  hash: {},
  codec: {},
};

/******************** bitArray.js ********************/

sjcl.bitArray = {

  concat: function (a1, a2) {
    if (a1.length === 0 || a2.length === 0) {
      return a1.concat(a2);
    }
    
    var last = a1[a1.length-1], shift = sjcl.bitArray.getPartial(last);
    if (shift === 32) {
      return a1.concat(a2);
    } else {
      return sjcl.bitArray._shiftRight(a2, shift, last|0, a1.slice(0,a1.length-1));
    }
  },

  bitLength: function (a) {
    var l = a.length, x;
    if (l === 0) { return 0; }
    x = a[l - 1];
    return (l-1) * 32 + sjcl.bitArray.getPartial(x);
  },

  partial: function (len, x, _end) {
    if (len === 32) { return x; }
    return (_end ? x|0 : x << (32-len)) + len * 0x10000000000;
  },

  getPartial: function (x) {
    return Math.round(x/0x10000000000) || 32;
  },

  _shiftRight: function (a, shift, carry, out) {
    var i, last2=0, shift2;
    if (out === undefined) { out = []; }
    
    for (; shift >= 32; shift -= 32) {
      out.push(carry);
      carry = 0;
    }
    if (shift === 0) {
      return out.concat(a);
    }
    
    for (i=0; i<a.length; i++) {
      out.push(carry | a[i]>>>shift);
      carry = a[i] << (32-shift);
    }
    last2 = a.length ? a[a.length-1] : 0;
    shift2 = sjcl.bitArray.getPartial(last2);
    out.push(sjcl.bitArray.partial(shift+shift2 & 31, (shift + shift2 > 32) ? carry : out.pop(),1));
    return out;
  },
};

/******************** codecBytes.js ********************/

sjcl.codec.bytes = {

  fromBits: function (arr) {
    var out = [], bl = sjcl.bitArray.bitLength(arr), i, tmp;
    for (i=0; i<bl/8; i++) {
      if ((i&3) === 0) {
        tmp = arr[i/4];
      }
      out.push(tmp >>> 24);
      tmp <<= 8;
    }
    return out;
  },

  toBits: function (bytes) {
    var out = [], i, tmp=0;
    for (i=0; i<bytes.length; i++) {
      tmp = tmp << 8 | bytes[i];
      if ((i&3) === 3) {
        out.push(tmp);
        tmp = 0;
      }
    }
    if (i&3) {
      out.push(sjcl.bitArray.partial(8*(i&3), tmp));
    }
    return out;
  }
};

/******************** sha256.js ********************/

sjcl.hash.sha256 = function (hash) {
  if (!this._key[0]) { this._precompute(); }
  if (hash) {
    this._h = hash._h.slice(0);
    this._buffer = hash._buffer.slice(0);
    this._length = hash._length;
  } else {
    this.reset();
  }
};

sjcl.hash.sha256.hash = function (data) {
  return (new sjcl.hash.sha256()).update(data).finalize();
};

sjcl.hash.sha256.prototype = {
  blockSize: 512,
   
  reset:function () {
    this._h = this._init.slice(0);
    this._buffer = [];
    this._length = 0;
    return this;
  },
  
  update: function (data) {
    if (typeof data === "string") {
      data = sjcl.codec.utf8String.toBits(data);
    }
    var i, b = this._buffer = sjcl.bitArray.concat(this._buffer, data),
        ol = this._length,
        nl = this._length = ol + sjcl.bitArray.bitLength(data);
    for (i = 512+ol & -512; i <= nl; i+= 512) {
      this._block(b.splice(0,16));
    }
    return this;
  },
  
  finalize:function () {
    var i, b = this._buffer, h = this._h;

    // Round out and push the buffer
    b = sjcl.bitArray.concat(b, [sjcl.bitArray.partial(1,1)]);
    
    // Round out the buffer to a multiple of 16 words, less the 2 length words.
    for (i = b.length + 2; i & 15; i++) {
      b.push(0);
    }
    
    // append the length
    b.push(Math.floor(this._length / 0x100000000));
    b.push(this._length | 0);

    while (b.length) {
      this._block(b.splice(0,16));
    }

    this.reset();
    return h;
  },

  _init:[],

  _key:[],

  _precompute: function () {
    var i = 0, prime = 2, factor;

    function frac(x) { return (x-Math.floor(x)) * 0x100000000 | 0; }

    outer: for (; i<64; prime++) {
      for (factor=2; factor*factor <= prime; factor++) {
        if (prime % factor === 0) {
          // not a prime
          continue outer;
        }
      }
      
      if (i<8) {
        this._init[i] = frac(Math.pow(prime, 1/2));
      }
      this._key[i] = frac(Math.pow(prime, 1/3));
      i++;
    }
  },
  
  _block:function (words) {  
    var i, tmp, a, b,
      w = words.slice(0),
      h = this._h,
      k = this._key,
      h0 = h[0], h1 = h[1], h2 = h[2], h3 = h[3],
      h4 = h[4], h5 = h[5], h6 = h[6], h7 = h[7];

    for (i=0; i<64; i++) {
      // load up the input word for this round
      if (i<16) {
        tmp = w[i];
      } else {
        a   = w[(i+1 ) & 15];
        b   = w[(i+14) & 15];
        tmp = w[i&15] = ((a>>>7  ^ a>>>18 ^ a>>>3  ^ a<<25 ^ a<<14) + 
                         (b>>>17 ^ b>>>19 ^ b>>>10 ^ b<<15 ^ b<<13) +
                         w[i&15] + w[(i+9) & 15]) | 0;
      }
      
      tmp = (tmp + h7 + (h4>>>6 ^ h4>>>11 ^ h4>>>25 ^ h4<<26 ^ h4<<21 ^ h4<<7) +  (h6 ^ h4&(h5^h6)) + k[i]); // | 0;
      
      // shift register
      h7 = h6; h6 = h5; h5 = h4;
      h4 = h3 + tmp | 0;
      h3 = h2; h2 = h1; h1 = h0;

      h0 = (tmp +  ((h1&h2) ^ (h3&(h1^h2))) + (h1>>>2 ^ h1>>>13 ^ h1>>>22 ^ h1<<30 ^ h1<<19 ^ h1<<10)) | 0;
    }

    h[0] = h[0]+h0 | 0;
    h[1] = h[1]+h1 | 0;
    h[2] = h[2]+h2 | 0;
    h[3] = h[3]+h3 | 0;
    h[4] = h[4]+h4 | 0;
    h[5] = h[5]+h5 | 0;
    h[6] = h[6]+h6 | 0;
    h[7] = h[7]+h7 | 0;
  }
};