<?php

/*
 * CoinSpark 1.0 - PHP library
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


//	Constants for use by clients of the library

	define('COINSPARK_SATOSHI_QTY_MAX', 2100000000000000);
	define('COINSPARK_ASSET_QTY_MAX', 100000000000000);
	define('COINSPARK_PAYMENT_REF_MAX', 4503599627370495); // 2^52-1
	
	define('COINSPARK_GENESIS_QTY_MANTISSA_MIN', 1);
	define('COINSPARK_GENESIS_QTY_MANTISSA_MAX', 1000);
	define('COINSPARK_GENESIS_QTY_EXPONENT_MIN', 0);
	define('COINSPARK_GENESIS_QTY_EXPONENT_MAX', 11);
	define('COINSPARK_GENESIS_CHARGE_FLAT_MAX', 5000);
	define('COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN', 0);
	define('COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX', 100);
	define('COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX', 50);
	define('COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN', 0);
	define('COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX', 2);
	define('COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN', 0);
	define('COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX', 250);
	define('COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN', 32);
	define('COINSPARK_GENESIS_PAGE_PATH_MAX_LEN', 24);
	define('COINSPARK_GENESIS_HASH_MIN_LEN', 12);
	define('COINSPARK_GENESIS_HASH_MAX_LEN', 32);

	define('COINSPARK_ASSETREF_BLOCK_NUM_MAX', 4294967295);
	define('COINSPARK_ASSETREF_TX_OFFSET_MAX', 4294967295);
	define('COINSPARK_ASSETREF_TXID_PREFIX_LEN', 2);

	define('COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE', -1); // magic number for a default route

	define('COINSPARK_IO_INDEX_MAX', 65535);
	
	define('COINSPARK_ADDRESS_FLAG_ASSETS', 1);
	define('COINSPARK_ADDRESS_FLAG_PAYMENT_REFS', 2);
	define('COINSPARK_ADDRESS_FLAG_MASK', 0x7FFFFF); // 23 bits are currently usable


//	Constants for internal use only
		
	define('COINSPARK_UNSIGNED_BYTE_MAX', 0xFF);
	define('COINSPARK_UNSIGNED_2_BYTES_MAX', 0xFFFF);
	define('COINSPARK_UNSIGNED_3_BYTES_MAX', 0xFFFFFF);
	define('COINSPARK_UNSIGNED_4_BYTES_MAX', 4294967295); // don't use hex notation for sake of 32-bit PHP

	define('COINSPARK_METADATA_IDENTIFIER', "SPK");
	define('COINSPARK_METADATA_IDENTIFIER_LEN', 3);
	define('COINSPARK_LENGTH_PREFIX_MAX', 96);
	define('COINSPARK_GENESIS_PREFIX', 'g');
	define('COINSPARK_TRANSFERS_PREFIX', 't');
	define('COINSPARK_PAYMENTREF_PREFIX', 'r');

	define('COINSPARK_FEE_BASIS_MAX_SATOSHIS', 1000);

	define('COINSPARK_INTEGER_TO_BASE_58', "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz");

	define('COINSPARK_DOMAIN_PACKING_PREFIX_MASK', 0xC0);
	define('COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT', 6);
	define('COINSPARK_DOMAIN_PACKING_SUFFIX_MASK', 0x3F);
	define('COINSPARK_DOMAIN_PACKING_SUFFIX_MAX', 62);
	define('COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4', 63);
	define('COINSPARK_DOMAIN_PACKING_IPv4_HTTPS', 0x40);

	define('COINSPARK_DOMAIN_PATH_ENCODE_BASE', 40);
	define('COINSPARK_DOMAIN_PATH_FALSE_END_CHAR', '<');
	define('COINSPARK_DOMAIN_PATH_TRUE_END_CHAR', '>');
	define('COINSPARK_DOMAIN_PATH_CHARS', "0123456789abcdefghijklmnopqrstuvwxyz-.<>");


//	Quasi-constants for internal use only (PHP doesn't allow array constants)
	
	$COINSPARK_DOMAIN_NAME_PREFIXES=array(
		"",
		"www."
	);
	
	$COINSPARK_DOMAIN_NAME_SUFFIXES=array(
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
	);
		

//	General public functions for managing CoinSpark metadata and bitcoin transaction output scripts

	/**
	 * Extracts OP_RETURN metadata (not necessarily CoinSpark data) from a bitcoin tx output script.
	 *
	 * @param string $scriptPubKey Output script as raw binary data or hexadecimal.
	 * @param boolean $scriptIsHex True to interpret $scriptPubKey as a hex string, false as raw binary.
	 *
	 * @return string|null Raw binary embedded metadata if found, null otherwise.
	 */

	function CoinSparkScriptToMetadata($scriptPubKey, $scriptIsHex)
	{
		$scriptPubKeyRaw=CoinSparkGetRawScript($scriptPubKey, $scriptIsHex);
		$scriptPubKeyRawLen=strlen($scriptPubKeyRaw);
		$metadataLen=$scriptPubKeyRawLen-2;
		
		if (
			($scriptPubKeyRawLen>2) &&
			(ord($scriptPubKeyRaw[0]) == 0x6a) &&
			(ord($scriptPubKeyRaw[1]) > 0) &&
			(ord($scriptPubKeyRaw[1]) <= 75) &&
			(ord($scriptPubKeyRaw[1]) == $metadataLen)
		)
			return substr($scriptPubKeyRaw, 2);
		
		return null;
	}
	

	/**
	 * Extracts first OP_RETURN metadata (not necessarily CoinSpark data) from an array of bitcoin tx output scripts.
	 *
	 * @param array $scriptPubKeys Array of output scripts as raw binary data or hexadecimal.
	 * @param boolean $scriptsAreHex True to interpret each element of $scriptPubKeys as a hex string, false as raw binary.
	 *
	 * @return string|null First raw binary embedded metadata if found, null if none found.
	 */

	function CoinSparkScriptsToMetadata($scriptPubKeys, $scriptsAreHex)
	{
		foreach ($scriptPubKeys as $scriptPubKey)
			if (!CoinSparkScriptIsRegular($scriptPubKey, $scriptsAreHex))
				return CoinSparkScriptToMetadata($scriptPubKey, $scriptsAreHex);
				
		return null;
	}
	

	/**
	 * Converts CoinSpark metadata (or other data) into an OP_RETURN bitcoin tx output script.
	 *
	 * @param string $metadata Raw binary metadata to be converted.
	 * @param boolean $toHexScript True to return a script as a hex string, false as raw binary.
	 *
	 * @return string|null The OP_RETURN bitcoin tx output script as hex or raw binary, null if we failed.
	 */

	function CoinSparkMetadataToScript($metadata, $toHexScript)
	{
		if (strlen($metadata)<=75) {
			$scriptPubKey=chr(0x6a).chr(strlen($metadata)).$metadata;
			if ($toHexScript)
				$scriptPubKey=strtoupper(bin2hex($scriptPubKey));
			
			return $scriptPubKey;
		}
			
		return null;
	}
	

	/**
	 * Calculates the maximum length of CoinSpark metadata that can be added to some existing CoinSpark metadata
	 * to fit into a specified number of bytes.
	 * 
	 * The calculation is not simply $metadataMaxLen-strlen(metadata) because some space is saved when combining pieces of CoinSpark metadata together.
	 *
	 * @param string $metadata The existing CoinSpark metadata in raw binary form, which can itself already be
	 *    a combination of more than one CoinSpark metadata element.
	 * @param integer $metadataMaxLen The total number of bytes available for the combined metadata.
	 *
	 * @return integer The number of bytes which are available for the new piece of metadata to be added.
	 */

	function CoinSparkMetadataMaxAppendLen($metadata, $metadataMaxLen)
	{
		return max($metadataMaxLen-(strlen($metadata)+1-COINSPARK_METADATA_IDENTIFIER_LEN), 0);
	}


	/**
	 * Appends one piece of CoinSpark metadata to another.
	 * 
	 * @param string $metadata The existing CoinSpark metadata in raw binary form, which can itself already be
	 *    a combination of more than one CoinSpark metadata element.
	 * @param integer $metadataMaxLen The total number of bytes available for the combined metadata.
	 * @param string $appendMetadata The new CoinSpark metadata to be appended, in raw binary form.
	 *
	 * @return string|null The comvined CoinSpark metadata as raw binary, or null if we failed.
	 */
	
	function CoinSparkMetadataAppend($metadata, $metadataMaxLen, $appendMetadata)
	{
		$lastMetadata=CoinSparkLocateMetadataRange($metadata, null); // check we can find last metadata
		if (!isset($lastMetadata))
			return null;

		if (strlen($appendMetadata)<(COINSPARK_METADATA_IDENTIFIER_LEN+1)) // check there is enough to check the prefix
			return null;
			
		if (substr($appendMetadata, 0, COINSPARK_METADATA_IDENTIFIER_LEN)!=COINSPARK_METADATA_IDENTIFIER) // then check the prefix
			return null;
		
		// we don't check the character after the prefix in $appendMetadata because it could itself be composite
		
		$needLength=strlen($metadata)+strlen($appendMetadata)-COINSPARK_METADATA_IDENTIFIER_LEN+1; // check there is enough space
		if ($metadataMaxLen<$needLength)
			return 0;
		
		$lastMetadataLen=strlen($lastMetadata)+1; // include prefix
		$lastMetadataPos=strlen($metadata)-$lastMetadataLen;
		
		return substr($metadata, 0, $lastMetadataPos).chr($lastMetadataLen).substr($metadata, $lastMetadataPos).
			substr($appendMetadata, COINSPARK_METADATA_IDENTIFIER_LEN);
	}
	

	/**
	 * Tests whether a bitcoin tx output script is 'regular', i.e. not an OP_RETURN script.
	 *
	 * This function will declare empty scripts or invalid hex scripts as 'regular' as well, since they are not OP_RETURNs.
	 * Use this to build $outputsRegular arrays which are used by various other functions.
	 * 
	 * @param string $scriptPubKey Output script as raw binary data or hexadecimal.
	 * @param boolean $scriptIsHex True to interpret $scriptPubKey as a hex string, false as raw binary.
	 *
	 * @return boolean True if the script is 'regular', false if it is an OP_RETURN script.
	 */
	
	function CoinSparkScriptIsRegular($scriptPubKey, $scriptIsHex)
    {
		$scriptPubKeyRaw=CoinSparkGetRawScript($scriptPubKey, $scriptIsHex);
			
    	return (strlen($scriptPubKeyRaw)<1) || (ord($scriptPubKeyRaw[0])!=0x6a);
    }
    
    
//	Function for calculating asset hashes
    
	/**
	 * Calculates the assetHash for the key information from a CoinSpark asset web page JSON specification.
	 *
	 * All parameters except $contractContent must be passed using UTF-8 encoding. You may pass null for any
	 * parameter which was not in the JSON, and this is equivalent to passing the empty string.
	 * 
	 * @param string $name
	 * @param string $issuer
	 * @param string $description
	 * @param string $units
	 * @param string|null $issueDate
	 * @param string|null $expiryDate
	 * @param string|float|integer|null $interestRate
	 * @param string|float|integer|null $multiple
	 * @param string $contractContent The contract *content*, not its URL.
	 *
	 * @return string The assetHash as a raw binary string.
	 */

	function CoinSparkCalcAssetHash($name, $issuer, $description, $units, $issueDate, $expiryDate, $interestRate, $multiple, $contractContent)
	{
		$mask="\x09\x0A\x0D\x20";
		
		$buffer=trim($name, $mask)."\x00";
		$buffer.=trim($issuer, $mask)."\x00";
		$buffer.=trim($description, $mask)."\x00";
		$buffer.=trim($units, $mask)."\x00";
		$buffer.=trim($issueDate, $mask)."\x00";
		$buffer.=trim($expiryDate, $mask)."\x00";
		
		$interestRateToHash=floor((isset($interestRate) ? $interestRate : 0)*1000000.0+0.5);
		$multipleToHash=floor((isset($multiple) ? $multiple : 1)*1000000.0+0.5);
		
		$buffer.=sprintf("%.0f", $interestRateToHash)."\x00";
		$buffer.=sprintf("%.0f", $multipleToHash)."\x00";
		
		$buffer.=$contractContent."\x00";
		
		return hash('sha256', $buffer, true);
	}
	

//	CoinSparkAddress class for managing CoinSpark addresseses

	class CoinSparkAddress extends CoinSparkBase {
		public $bitcoinAddress; // string
		public $addressFlags; // integer
		public $paymentRef; // CoinSparkPaymentRef object
		
		const COINSPARK_ADDRESS_PREFIX='s';
		const COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE=10;
		const COINSPARK_ADDRESS_CHAR_INCREMENT=13;
		
		function __construct()
		{
			$this->clear();
		}
		

		/**
		 * Sets all fields to their default/zero values, which are not necessarily valid.
		 */

		function clear()
		{
			$this->bitcoinAddress='';
			$this->addressFlags=0;
			$this->paymentRef=new CoinSparkPaymentRef();
		}
		

		/**
		 * Converts the CoinSparkAddress into a string for debugging.
		 *
		 * @return string
		 */

		function toString()
		{
			$flagsToStrings=array(
				COINSPARK_ADDRESS_FLAG_ASSETS => "assets",
				COINSPARK_ADDRESS_FLAG_PAYMENT_REFS => "payment references",
			);
			
			$buffer="COINSPARK ADDRESS\n";
			$buffer.=sprintf("  Bitcoin address: %s\n", $this->bitcoinAddress);
			$buffer.=sprintf("    Address flags: %d", $this->addressFlags);
			
			$flagOutput=false;
			
			foreach ($flagsToStrings as $flag => $string)
				if ($this->addressFlags & $flag) {
					$buffer.=($flagOutput ? ", " : " [").$string;
					$flagOutput=true;
				}
				
			$buffer.=($flagOutput ? "]" : "")."\n";
	
			$buffer.=sprintf("Payment reference: %.0f\n", $this->paymentRef->ref);
			$buffer.="END COINSPARK ADDRESS\n\n";

			return $buffer;
		}
		

		/**
		 * Tests whether all member values are in their permitted ranges.
		 *
		 * @return boolean
		 */

		function isValid()
		{
			if ( (!$this->isString($this->bitcoinAddress)) || !strlen($this->bitcoinAddress) )
				return false;
			
			if ( (($this->addressFlags&COINSPARK_ADDRESS_FLAG_MASK) != $this->addressFlags) || !$this->isInteger($this->addressFlags) )
				return false;
				
			return $this->paymentRef->isValid();
		}
		

		/**
		 * Tests whether this CoinSparkAddress has identical values to another.
		 *
		 * @param CoinSparkAddress $otherAddress The address to compare against this one.
		 *
		 * @return boolean
		 */

		function match($otherAddress)
		{
			return (!strcmp($this->bitcoinAddress, $otherAddress->bitcoinAddress)) &&
				($this->addressFlags==$otherAddress->addressFlags) && $this->paymentRef->match($otherAddress->paymentRef);
		}
		

		/**
		 * Encodes the CoinSparkAddress as an address string.
		 *
		 * @return string|null The address string, or null if the encoding failed.
		 */

		function encode()
		{
			if (!$this->isValid())
				return null;
				
			$stringBase58=array();

		//	Build up extra data for address flags
		
			$addressFlagChars=0;
			$testAddressFlags=$this->addressFlags;
			
			while ($testAddressFlags>0) {
				$stringBase58[2+$addressFlagChars]=$testAddressFlags%58;
				$testAddressFlags=(int)($testAddressFlags/58); // treat as an integer
				$addressFlagChars++;
			}
			
		//	Build up extra data for payment reference
			
			$paymentRefChars=0;
			$testPaymentRef=$this->paymentRef->ref;
			
			while ($testPaymentRef>0) {
				$stringBase58[2+$addressFlagChars+$paymentRefChars]=(int)round(fmod($testPaymentRef, 58));
				$testPaymentRef=floor($testPaymentRef/58);
				$paymentRefChars++;
			}
			
		//	Calculate and encode extra length 
		
			$extraDataChars=$addressFlagChars+$paymentRefChars;
			$bitcoinAddressLen=strlen($this->bitcoinAddress);
			$stringLen=$bitcoinAddressLen+2+$extraDataChars;

			$stringBase58[1]=$addressFlagChars*self::COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE+$paymentRefChars;
			
		//  Convert the bitcoin address
	
			for ($charIndex=0; $charIndex<$bitcoinAddressLen; $charIndex++) {
				$charValue=$this->base58ToInteger($this->bitcoinAddress[$charIndex]);
				if (!isset($charValue))
					return null; // invalid base58 character
		
				$charValue+=self::COINSPARK_ADDRESS_CHAR_INCREMENT;
		
				if ($extraDataChars>0)
					$charValue+=$stringBase58[2+$charIndex%$extraDataChars];
				
				$stringBase58[2+$extraDataChars+$charIndex]=$charValue%58;
			}
			
		//  Obfuscate first half of address using second half to prevent common prefixes
		
			$halfLength=ceil($stringLen/2);
			for ($charIndex=1; $charIndex<$halfLength; $charIndex++) // exclude first character
				$stringBase58[$charIndex]=($stringBase58[$charIndex]+$stringBase58[$stringLen-$charIndex])%58;
				
		//	Convert to base 58 and add prefix
		
			$string=self::COINSPARK_ADDRESS_PREFIX;
			$integerToBase58=COINSPARK_INTEGER_TO_BASE_58; // can't index directly into constant
			for ($charIndex=1; $charIndex<$stringLen; $charIndex++)
				$string.=$integerToBase58[$stringBase58[$charIndex]];
		
			return $string;
		}
		

		/**
		 * Decodes an address string into this CoinSparkAddress.
		 *
		 * @return boolean True if the decoding was successful, false otherwise.
		 */

		function decode($string)
		{

		//  Check for basic validity
	
			$stringLen=strlen($string);
			if ($stringLen<2)
				return false;
	
			if ($string[0]!=self::COINSPARK_ADDRESS_PREFIX)
				return false;
				
		//	Convert from base 58
		
			$stringBase58=array();
			for ($charIndex=1; $charIndex<$stringLen; $charIndex++) { // exclude first character
				$charValue=$this->base58ToInteger($string[$charIndex]);
				if (!isset($charValue))
					return false;
				$stringBase58[$charIndex]=$charValue;
			}
			
		//	De-obfuscate first half of address using second half
		
			$halfLength=ceil($stringLen/2);
			for ($charIndex=1; $charIndex<$halfLength; $charIndex++) // exclude first character
				$stringBase58[$charIndex]=($stringBase58[$charIndex]+58-$stringBase58[$stringLen-$charIndex])%58;
				
		//	Get length of extra data

			$charValue=$stringBase58[1];
			$addressFlagChars=(int)($charValue/self::COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE);
			$paymentRefChars=$charValue%self::COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE;
			$extraDataChars=$addressFlagChars+$paymentRefChars;
			
			if ($stringLen<(2+$extraDataChars))
				return false;
			
			$bitcoinAddressLen=$stringLen-2-$extraDataChars;
	
		//  Read the extra data for address flags
	
			$this->addressFlags=0;
			$multiplier=1;
	
			for ($charIndex=0; $charIndex<$addressFlagChars; $charIndex++) {
				$charValue=$stringBase58[2+$charIndex];
				$this->addressFlags+=$charValue*$multiplier;
				$multiplier*=58;
			}
			
		//	Read the extra data for payment reference
		
			$this->paymentRef->ref=0;
			$multiplier=1;
			
			for ($charIndex=0; $charIndex<$paymentRefChars; $charIndex++) {
				$charValue=$stringBase58[2+$addressFlagChars+$charIndex];
				$this->paymentRef->ref+=$charValue*$multiplier;
				$multiplier*=58;
			}
		
		//  Convert the bitcoin address
	
			$integerToBase58=COINSPARK_INTEGER_TO_BASE_58; // can't index directly into constant
			$this->bitcoinAddress='';
	
			for ($charIndex=0; $charIndex<$bitcoinAddressLen; $charIndex++) {
				$charValue=$stringBase58[2+$extraDataChars+$charIndex];
				$charValue+=58*2-self::COINSPARK_ADDRESS_CHAR_INCREMENT; // avoid worrying about the result of modulo on negative numbers in any language
		
				if ($extraDataChars>0)
					$charValue-=$stringBase58[2+$charIndex%$extraDataChars];
		
				$this->bitcoinAddress.=$integerToBase58[$charValue%58];
			}
	
			return $this->isValid();
		}
	}
	

//	CoinSparkGenesis class for managing asset genesis metadata

	class CoinSparkGenesis extends CoinSparkBase {
		public $qtyMantissa; // integer
		public $qtyExponent; // integer
		public $chargeFlatMantissa; // integer
		public $chargeFlatExponent; // integer
		public $chargeBasisPoints; // integer
		public $useHttps; // boolean
		public $domainName; // string
		public $usePrefix; // boolean
		public $pagePath; // string
		public $assetHash; // raw binary string
		public $assetHashLen; // number of bytes in assetHash that are valid for comparison
		
		const COINSPARK_GENESIS_QTY_FLAGS_LENGTH=2;
		const COINSPARK_GENESIS_QTY_MASK=0x3FFF;
		const COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE=1001;
		const COINSPARK_GENESIS_FLAG_CHARGE_FLAT=0x4000;
		const COINSPARK_GENESIS_FLAG_CHARGE_BPS=0x8000;
		const COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE=101;
		const COINSPARK_GENESIS_CHARGE_FLAT_LENGTH=1;
		const COINSPARK_GENESIS_CHARGE_BPS_LENGTH=1;
		
		function __construct()
		{
			$this->clear();
		}
		
		function clear()
		{
			$this->qtyMantissa=0;
			$this->qtyExponent=0;
			$this->chargeFlatMantissa=0;
			$this->chargeFlatExponent=0;
			$this->chargeBasisPoints=0;
			$this->useHttps=false;
			$this->domainName='';
			$this->usePrefix=true;
			$this->pagePath='';
			$this->assetHash='';
			$this->assetHashLen=0;
		}
		
		function toString()
		{
			$quantity=$this->getQty();
			$quantityEncoded=($this->qtyExponent*self::COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+$this->qtyMantissa)&
				self::COINSPARK_GENESIS_QTY_MASK;
			$chargeFlat=$this->getChargeFlat();
			$chargeFlatEncoded=$this->chargeFlatExponent*self::COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+$this->chargeFlatMantissa;
			$domainPathMetadata=$this->encodeDomainAndOrPath($this->domainName, $this->useHttps, $this->pagePath, $this->usePrefix);
			
			$buffer="COINSPARK GENESIS\n";
			$buffer.=sprintf("   Quantity mantissa: %d\n", $this->qtyMantissa);
			$buffer.=sprintf("   Quantity exponent: %d\n", $this->qtyExponent);
			$buffer.=sprintf("    Quantity encoded: %d (small endian hex %s)\n", $quantityEncoded,
				$this->unsignedToSmallEndianHex($quantityEncoded, self::COINSPARK_GENESIS_QTY_FLAGS_LENGTH));
			$buffer.=sprintf("      Quantity value: %.0f\n", $quantity);
			$buffer.=sprintf("Flat charge mantissa: %d\n", $this->chargeFlatMantissa);
			$buffer.=sprintf("Flat charge exponent: %d\n", $this->chargeFlatExponent);
			$buffer.=sprintf(" Flat charge encoded: %d (small endian hex %s)\n", $chargeFlatEncoded,
				$this->unsignedToSmallEndianHex($chargeFlatEncoded, self::COINSPARK_GENESIS_CHARGE_FLAT_LENGTH));
			$buffer.=sprintf("   Flat charge value: %.0f\n", $chargeFlat);
			$buffer.=sprintf(" Basis points charge: %d (hex %s)\n", $this->chargeBasisPoints,
				$this->unsignedToSmallEndianHex($this->chargeBasisPoints, self::COINSPARK_GENESIS_CHARGE_BPS_LENGTH));
			$buffer.=sprintf("           Asset URL: %s://%s/%s%s/ (length %d+%d encoded %s length %d)\n",
				$this->useHttps ? 'https' : 'http', $this->domainName,
				$this->usePrefix ? "coinspark/" : "", strlen($this->pagePath) ? $this->pagePath : "[spent-txid]",
				strlen($this->domainName), strlen($this->pagePath),
				strtoupper(bin2hex($domainPathMetadata)), strlen($domainPathMetadata));
			$buffer.=sprintf("          Asset hash: %s (length %d)\n", strtoupper(bin2hex(substr($this->assetHash, 0, $this->assetHashLen))), $this->assetHashLen);
			$buffer.="END COINSPARK GENESIS\n\n";
			
			return $buffer;
		}
		
		function isValid()
		{
			if (!(
				$this->isInteger($this->qtyMantissa) &&
				$this->isInteger($this->qtyExponent) &&
				$this->isInteger($this->chargeFlatMantissa) &&
				$this->isInteger($this->chargeFlatExponent) &&
				$this->isInteger($this->chargeBasisPoints) &&
				$this->isBoolean($this->useHttps) &&
				$this->isString($this->domainName) &&
				$this->isBoolean($this->usePrefix) &&
				$this->isString($this->pagePath) &&
				$this->isString($this->assetHash) &&
				$this->isInteger($this->assetHashLen)
			))
				return false;
			
			if ( ($this->qtyMantissa<COINSPARK_GENESIS_QTY_MANTISSA_MIN) || ($this->qtyMantissa>COINSPARK_GENESIS_QTY_MANTISSA_MAX) )
				return false;
				
			if ( ($this->qtyExponent<COINSPARK_GENESIS_QTY_EXPONENT_MIN) || ($this->qtyExponent>COINSPARK_GENESIS_QTY_EXPONENT_MAX) )
				return false;
			
			if ( ($this->chargeFlatExponent<COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN) || ($this->chargeFlatExponent>COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) )
				return false;
				
			if ($this->chargeFlatMantissa<COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN)
				return false;
				
			if ($this->chargeFlatMantissa > (($this->chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) ? COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX : COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX))
				return false;
				
		    if ( ($this->chargeBasisPoints<COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN) || ($this->chargeBasisPoints>COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX) )
		    	return false;
				
			if (strlen($this->domainName)>COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN)
				return false;
				
			if (strlen($this->pagePath)>COINSPARK_GENESIS_PAGE_PATH_MAX_LEN)
				return false;
				
			if (strlen($this->assetHash)<$this->assetHashLen) // check we have at least as much data as specified by $this->assetHashLen
				return false; 
			
			if ( ($this->assetHashLen<COINSPARK_GENESIS_HASH_MIN_LEN) || ($this->assetHashLen>COINSPARK_GENESIS_HASH_MAX_LEN) )
				return false;
				
			return true;
		}
		
		function match($otherGenesis, $strict)
		{
			$hashCompareLen=min($this->assetHashLen, $otherGenesis->assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN);
			
			if ($strict)
				$floatQuantitiesMatch=($this->qtyMantissa==$otherGenesis->qtyMantissa) && ($this->qtyExponent==$otherGenesis->qtyExponent) &&
					($this->chargeFlatMantissa==$otherGenesis->chargeFlatMantissa) && ($this->chargeFlatExponent==$otherGenesis->chargeFlatExponent);
			else
				$floatQuantitiesMatch=($this->getQty()==$otherGenesis->getQty()) && ($this->getChargeFlat()==$otherGenesis->getChargeFlat());
			
			return $floatQuantitiesMatch && ($this->chargeBasisPoints==$otherGenesis->chargeBasisPoints) &&
				($this->useHttps==$otherGenesis->useHttps) &&
				(!strcasecmp($this->domainName, $otherGenesis->domainName)) &&
				($this->usePrefix==$otherGenesis->usePrefix) &&
				(!strcasecmp($this->pagePath, $otherGenesis->pagePath)) &&
				(!strncasecmp($this->assetHash, $otherGenesis->assetHash, $hashCompareLen));
		}

		function getQty()
		{
			return $this->mantissaExponentToQty($this->qtyMantissa, $this->qtyExponent);
		}
		
		function setQty($desiredQty, $rounding)
		{
			$this->qtyToMantissaExponent($desiredQty, $rounding, COINSPARK_GENESIS_QTY_MANTISSA_MAX,
				COINSPARK_GENESIS_QTY_EXPONENT_MAX, $this->qtyMantissa, $this->qtyExponent);
				
			return $this->getQty();
		}
		
		function getChargeFlat()
		{
			return $this->mantissaExponentToQty($this->chargeFlatMantissa, $this->chargeFlatExponent);
		}
		
		function setChargeFlat($desiredChargeFlat, $rounding)
		{
			$this->qtyToMantissaExponent($desiredChargeFlat, $rounding, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX,
				COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX, $this->chargeFlatMantissa, $this->chargeFlatExponent);
				
			if ($this->chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX)
				$this->chargeFlatMantissa=min($this->chargeFlatMantissa, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX);
				
			return $this->getChargeFlat();
		}
		
		function calcCharge($qtyGross)
		{
			$charge=$this->getChargeFlat()+floor(($qtyGross*$this->chargeBasisPoints+5000)/10000); // rounds to nearest
			
			return min($qtyGross, $charge);
		}
		
		function calcNet($qtyGross)
		{
			return $qtyGross-$this->calcCharge($qtyGross);
		}
		
		function calcGross($qtyNet)
		{
			if ($qtyNet<=0)
				return 0; // no point getting past charges if we end up with zero anyway
				
			$lowerGross=floor((($qtyNet+$this->getChargeFlat())*10000)/(10000-$this->chargeBasisPoints)); // divides rounding down
			
			return ($this->calcNet($lowerGross)>=$qtyNet) ? $lowerGross : ($lowerGross+1);
		}
		
		function calcHashLen($metadataMaxLen)
		{
			$assetHashLen=$metadataMaxLen-COINSPARK_METADATA_IDENTIFIER_LEN-1-self::COINSPARK_GENESIS_QTY_FLAGS_LENGTH;
			
			if ($this->chargeFlatMantissa>0)
				$assetHashLen-=self::COINSPARK_GENESIS_CHARGE_FLAT_LENGTH;
				
			if ($this->chargeBasisPoints>0)
				$assetHashLen-=self::COINSPARK_GENESIS_CHARGE_BPS_LENGTH;
			
			$domainPathLen=strlen($this->pagePath)+1;
				
			if ($this->readIPv4Address($this->domainName))
				$assetHashLen-=5; // packing and IP octets
			else {
				$assetHashLen-=1; // packing
				$domainPathLen+=strlen($this->shrinkLowerDomainName($this->domainName, $packing))+1;
			}
			
			$assetHashLen-=2*floor(($domainPathLen+2)/3); // uses integer arithmetic
			
			return min($assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN);
		}
		
		function encode($metadataMaxLen)
		{
			if (!$this->isValid())
				return null;

		//  4-character identifier
				
			$metadata=COINSPARK_METADATA_IDENTIFIER.COINSPARK_GENESIS_PREFIX;

		//	Quantity mantissa and exponent
		
			$quantityEncoded=($this->qtyExponent*self::COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+$this->qtyMantissa)&self::COINSPARK_GENESIS_QTY_MASK;
			if ($this->chargeFlatMantissa>0)
				$quantityEncoded|=self::COINSPARK_GENESIS_FLAG_CHARGE_FLAT;
			if ($this->chargeBasisPoints>0)
				$quantityEncoded|=self::COINSPARK_GENESIS_FLAG_CHARGE_BPS;

			$written=$this->writeSmallEndianUnsigned($quantityEncoded, self::COINSPARK_GENESIS_QTY_FLAGS_LENGTH);
			if (!isset($written))
				return null;
			
			$metadata.=$written;
			
		//	Charges - flat and basis points
		
			if ($quantityEncoded & self::COINSPARK_GENESIS_FLAG_CHARGE_FLAT) {
				$chargeEncoded=$this->chargeFlatExponent*self::COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+$this->chargeFlatMantissa;
			
				$written=$this->writeSmallEndianUnsigned($chargeEncoded, self::COINSPARK_GENESIS_CHARGE_FLAT_LENGTH);
				if (!isset($written))
					return null;
					
				$metadata.=$written;
			}
	
			if ($quantityEncoded & self::COINSPARK_GENESIS_FLAG_CHARGE_BPS) {
				$written=$this->writeSmallEndianUnsigned($this->chargeBasisPoints, self::COINSPARK_GENESIS_CHARGE_BPS_LENGTH);
				if (!isset($written))
					return null;
					
				$metadata.=$written;
			}
			
		//	Domain name and page path
		
			$written=$this->encodeDomainAndOrPath($this->domainName, $this->useHttps, $this->pagePath, $this->usePrefix);
			if (!isset($written))
				return null;
	
			$metadata.=$written;

		//	Asset hash
		
			$metadata.=substr($this->assetHash, 0, $this->assetHashLen);
			
		//	Check the total length is within the specified limit
		
			if (strlen($metadata)>$metadataMaxLen)
				return null;
				
		//	Return what we created
		
			return $metadata;
		}
		
		function decode($metadata)
		{
			$metadata=CoinSparkLocateMetadataRange($metadata, COINSPARK_GENESIS_PREFIX);
			if (!isset($metadata))
				return false;
			
		//	Quantity mantissa and exponent
		
			$quantityEncoded=$this->shiftReadSmallEndianUnsigned($metadata, self::COINSPARK_GENESIS_QTY_FLAGS_LENGTH);
			if (!isset($quantityEncoded))
				return false;
				
			$this->qtyMantissa=($quantityEncoded&self::COINSPARK_GENESIS_QTY_MASK)%self::COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE;
			$this->qtyExponent=(int)(($quantityEncoded&self::COINSPARK_GENESIS_QTY_MASK)/self::COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE);
		
		//	Charges - flat and basis points
		
			if ($quantityEncoded & self::COINSPARK_GENESIS_FLAG_CHARGE_FLAT) {
				$chargeEncoded=$this->shiftReadSmallEndianUnsigned($metadata, self::COINSPARK_GENESIS_CHARGE_FLAT_LENGTH);
				
				$this->chargeFlatMantissa=$chargeEncoded%self::COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE;
				$this->chargeFlatExponent=(int)($chargeEncoded/self::COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE);
			
			} else {
				$this->chargeFlatMantissa=0;
				$this->chargeFlatExponent=0;
			}
			
			if ($quantityEncoded & self::COINSPARK_GENESIS_FLAG_CHARGE_BPS)
				$this->chargeBasisPoints=$this->shiftReadSmallEndianUnsigned($metadata, self::COINSPARK_GENESIS_CHARGE_BPS_LENGTH);
			else
				$this->chargeBasisPoints=0;
		
		//	Domain name and page path
			
			$decodedDomainPath=$this->shiftDecodeDomainAndOrPath($metadata, true, true);
			if (!isset($decodedDomainPath))
				return false;
				
			$this->useHttps=$decodedDomainPath['useHttps'];
			$this->domainName=$decodedDomainPath['domainName'];
			$this->usePrefix=$decodedDomainPath['usePrefix'];
			$this->pagePath=$decodedDomainPath['pagePath'];
			
		//	Asset hash
		
			$this->assetHashLen=min(strlen($metadata), COINSPARK_GENESIS_HASH_MAX_LEN);
			$this->assetHash=$this->stringShift($metadata, $this->assetHashLen);
			
		//	Return validity
		
			return $this->isValid();
		}
		
		function calcMinFee($outputsSatoshis, $outputsRegular)
		{
			if (count($outputsSatoshis)!=count($outputsRegular))
				return COINSPARK_SATOSHI_QTY_MAX; // these two arrays must be the same size

			return $this->countNonLastRegularOutputs($outputsRegular)*$this->getMinFeeBasis($outputsSatoshis, $outputsRegular);
		}
		
		function apply($outputsRegular)
		{
			$countOutputs=count($outputsRegular);
			$lastRegularOutput=$this->getLastRegularOutput($outputsRegular);
			$divideOutputs=$this->countNonLastRegularOutputs($outputsRegular);
			$genesisQty=$this->getQty();
			
			if ($divideOutputs==0)
				$qtyPerOutput=0;
			else
				$qtyPerOutput=floor($genesisQty/$divideOutputs); // rounds down
				
			$extraFirstOutput=$genesisQty-$qtyPerOutput*$divideOutputs;
			$outputBalances=array_fill(0, $countOutputs, 0);
			
			for ($outputIndex=0; $outputIndex<$countOutputs; $outputIndex++)
				if ($outputsRegular[$outputIndex] && ($outputIndex!=$lastRegularOutput)) {
					$outputBalances[$outputIndex]=$qtyPerOutput+$extraFirstOutput;
					$extraFirstOutput=0; // so it will only contribute to the first
				}
			
			return $outputBalances;
		}
		
		function calcAssetURL($firstSpentTxID, $firstSpentVout)
		{
			$firstSpentTxIdPart=substr($firstSpentTxID.$firstSpentTxID, $firstSpentVout%64, 16);
			
			return strtolower(
				(($this->useHttps) ? 'https' : 'http').
				'://'.$this->domainName.'/'.
				($this->usePrefix ? 'coinspark/' : '').
				(strlen($this->pagePath) ? $this->pagePath : $firstSpentTxIdPart).'/'
			);
		}
	}


//	CoinSparkAssetRef class for managing asset references
	
	class CoinSparkAssetRef extends CoinSparkBase {
		public $blockNum; // block in which genesis transaction is confirmed (number, can be float on 32-bit PHP)
		public $txOffset; // byte offset within that block (number, can be float on 32-bit PHP)
		public $txIDPrefix; // first COINSPARK_ASSETREF_TXID_PREFIX_LEN bytes of genesis transaction id (hexadecimal)
		
		function __construct()
		{
			$this->clear();
		}
		
		function clear()
		{
			$this->blockNum=0;
			$this->txOffset=0;
			$this->txIDPrefix=str_repeat('00', COINSPARK_ASSETREF_TXID_PREFIX_LEN);
		}
		
		function toString()
		{
			return $this->toStringInner(true);
		}
		
		function isValid()
		{
			if ($this->blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) {
				if ( ($this->blockNum<0) || ($this->blockNum>COINSPARK_ASSETREF_BLOCK_NUM_MAX) || !$this->isInteger($this->blockNum) )
					return false;
			
				if ( ($this->txOffset<0) || ($this->txOffset>COINSPARK_ASSETREF_TX_OFFSET_MAX) || !$this->isInteger($this->txOffset) )
					return false;
				
				if ( (!$this->isString($this->txIDPrefix)) || (strlen($this->txIDPrefix)!=(2*COINSPARK_ASSETREF_TXID_PREFIX_LEN)) )
					return false;
				
				if (strcasecmp(ltrim(dechex(hexdec($this->txIDPrefix)), '0'), ltrim($this->txIDPrefix, '0')))
					return false;
			}
				
			return true;
		}
		
		function match($otherAssetRef)
		{
			return (!strcasecmp($this->txIDPrefix, $otherAssetRef->txIDPrefix)) &&
				($this->txOffset == $otherAssetRef->txOffset) && ($this->blockNum == $otherAssetRef->blockNum);
		}
		
		function encode()
		{
			if (!$this->isValid())
				return null;
			
			$txIDPrefixInteger=256*hexdec(substr($this->txIDPrefix, 2, 2))+hexdec(substr($this->txIDPrefix, 0, 2));
			
			return sprintf("%.0f-%.0f-%d", $this->blockNum, $this->txOffset, $txIDPrefixInteger);
		}
		
		function decode($string)
		{
			if (strpos($string, '+')) // special check for '+' character which would be accepted by sscanf() below
				return false;
				
			if (sscanf($string, "%f-%f-%d", $this->blockNum, $this->txOffset, $txIDPrefixInteger)!=3)
				return false;
				
			if ( ($txIDPrefixInteger<0) || ($txIDPrefixInteger>0xFFFF) )
				return false;
				
			$this->txIDPrefix=sprintf("%02X%02X", $txIDPrefixInteger%256, floor($txIDPrefixInteger/256));
			
			return $this->isValid();
		}

		function toStringInner($headers)
		{
			$buffer=$headers ? "COINSPARK ASSET REFERENCE\n" : "";
	
			$buffer.=sprintf("Genesis block index: %.0f (small endian hex %s)\n", $this->blockNum,
				$this->unsignedToSmallEndianHex($this->blockNum, 4));
			$buffer.=sprintf(" Genesis txn offset: %.0f (small endian hex %s)\n", $this->txOffset,
				$this->unsignedToSmallEndianHex($this->txOffset, 4));
			$buffer.=sprintf("Genesis txid prefix: %s\n", strtoupper($this->txIDPrefix));
	
			if ($headers)
				$buffer.="END COINSPARK ASSET REFERENCE\n\n";

			return $buffer;
		}
		
		function compare($otherAssetRef)
		{
			// -1 if $this<$otherAssetRef, 1 if $otherAssetRef>$this, 0 otherwise
			
			if ($this->blockNum!=$otherAssetRef->blockNum)
				return ($this->blockNum<$otherAssetRef->blockNum) ? -1 : 1;
			else if ($this->blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) // in this case don't compare other fields
				return 0;
			else if ($this->txOffset!=$otherAssetRef->txOffset)
				return ($this->txOffset<$otherAssetRef->txOffset) ? -1 : 1;
			else
				return strncasecmp($this->txIDPrefix, $otherAssetRef->txIDPrefix, 2*COINSPARK_ASSETREF_TXID_PREFIX_LEN); // comparing hex gives same order as comparing bytes
		}		
	}


//	CoinSparkTransfer class for managing individual asset transfer metadata
	
	class CoinSparkTransfer extends CoinSparkBase {
		public $assetRef; // CoinSparkAssetRef object
		public $inputs; // CoinSparkIORange object
		public $outputs; // CoinSparkIORange object
		public $qtyPerOutput; // integer
		
		const COINSPARK_PACKING_GENESIS_MASK=0xC0;
		const COINSPARK_PACKING_GENESIS_PREV=0x00;
		const COINSPARK_PACKING_GENESIS_3_3_BYTES=0x40; // 3 bytes for block index, 3 for txn offset
		const COINSPARK_PACKING_GENESIS_3_4_BYTES=0x80; // 3 bytes for block index, 4 for txn offset
		const COINSPARK_PACKING_GENESIS_4_4_BYTES=0xC0; // 4 bytes for block index, 4 for txn offset
		
		const COINSPARK_PACKING_INDICES_MASK=0x38;
		const COINSPARK_PACKING_INDICES_0P_0P=0x00; // input 0 only or previous, output 0 only or previous
		const COINSPARK_PACKING_INDICES_0P_1S=0x08; // input 0 only or previous, output 1 only or subsequent single
		const COINSPARK_PACKING_INDICES_0P_ALL=0x10; // input 0 only or previous, all outputs
		const COINSPARK_PACKING_INDICES_1S_0P=0x18; // input 1 only or subsequent single, output 0 only or previous
		const COINSPARK_PACKING_INDICES_ALL_0P=0x20; // all inputs, output 0 only or previous
		const COINSPARK_PACKING_INDICES_ALL_1S=0x28; // all inputs, output 1 only or subsequent single
		const COINSPARK_PACKING_INDICES_ALL_ALL=0x30; // all inputs, all outputs
		const COINSPARK_PACKING_INDICES_EXTEND=0x38; // use second byte for more extensive information
		
		const COINSPARK_PACKING_EXTEND_INPUTS_SHIFT=3;
		const COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT=0;

		const COINSPARK_PACKING_EXTEND_MASK=0x07;
		const COINSPARK_PACKING_EXTEND_0P=0x00; // index 0 only or previous
		const COINSPARK_PACKING_EXTEND_1S=0x01; // index 1 only or subsequent single
		const COINSPARK_PACKING_EXTEND_BYTE=0x02; // 1 byte for single index
		const COINSPARK_PACKING_EXTEND_2_BYTES=0x03; // 2 bytes for single index
		const COINSPARK_PACKING_EXTEND_1_1_BYTES=0x04; // 1 byte for first index, 1 byte for count
		const COINSPARK_PACKING_EXTEND_2_1_BYTES=0x05; // 2 bytes for first index, 1 byte for count
		const COINSPARK_PACKING_EXTEND_2_2_BYTES=0x06; // 2 bytes for first index, 2 bytes for count
		const COINSPARK_PACKING_EXTEND_ALL=0x07; // all inputs|outputs

		const COINSPARK_PACKING_QUANTITY_MASK=0x07;
		const COINSPARK_PACKING_QUANTITY_1P=0x00; // quantity=1 or previous
		const COINSPARK_PACKING_QUANTITY_1_BYTE=0x01;
		const COINSPARK_PACKING_QUANTITY_2_BYTES=0x02;
		const COINSPARK_PACKING_QUANTITY_3_BYTES=0x03;
		const COINSPARK_PACKING_QUANTITY_4_BYTES=0x04;
		const COINSPARK_PACKING_QUANTITY_6_BYTES=0x05;
		const COINSPARK_PACKING_QUANTITY_FLOAT=0x06;
		const COINSPARK_PACKING_QUANTITY_MAX=0x07; // transfer all quantity across
		
		const COINSPARK_TRANSFER_QTY_FLOAT_LENGTH=2;
		const COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX=1000;
		const COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX=11;
		const COINSPARK_TRANSFER_QTY_FLOAT_MASK=0x3FFF;
		const COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE=1001;
		
		function __construct()
		{
			$this->clear();
		}
		
		function clear()
		{
			$this->assetRef=new CoinSparkAssetRef();
			$this->inputs=new CoinSparkIORange();
			$this->outputs=new CoinSparkIORange();
			$this->qtyPerOutput=0;
		}
		
		function toString()
		{
			return $this->toStringInner(true);
		}
		
		function isValid()
		{
			if (!($this->assetRef->isValid() && $this->inputs->isValid() && $this->outputs->isValid()))
				return false;
				
			if ( ($this->qtyPerOutput<0) || ($this->qtyPerOutput>COINSPARK_ASSET_QTY_MAX) || !$this->isInteger($this->qtyPerOutput) )
				return false;
				
			return true;
		}
		
		function match($otherTransfer)
		{
			if ($this->assetRef->blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
				return ($otherTransfer->assetRef->blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) &&
					$this->inputs->match($otherTransfer->inputs) && ($this->outputs->first==$otherTransfer->outputs->first);
			
			else
				return $this->assetRef->match($otherTransfer->assetRef) &&
					$this->inputs->match($otherTransfer->inputs) &&
					$this->outputs->match($otherTransfer->outputs) &&
					$this->qtyPerOutput==$otherTransfer->qtyPerOutput;
		}
		
		function encode($previousTransfer, $metadataMaxLen, $countInputs, $countOutputs)
		{
			if (!$this->isValid())
				return null;
			
			$packing=0;
			$isDefaultRoute=($this->assetRef->blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);
			
		//  Packing for genesis reference

			if ($isDefaultRoute) {
				if (isset($previousTransfer) && ($previousTransfer->assetRef->blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE))
					return null; // default route transfers have to come at the start
					
				$packing|=self::COINSPARK_PACKING_GENESIS_PREV;
				
			} else {
				if (isset($previousTransfer) && $this->assetRef->match($previousTransfer->assetRef))
					$packing|=self::COINSPARK_PACKING_GENESIS_PREV;
				
				elseif ($this->assetRef->blockNum <= COINSPARK_UNSIGNED_3_BYTES_MAX) {
					if ($this->assetRef->txOffset <= COINSPARK_UNSIGNED_3_BYTES_MAX)
						$packing|=self::COINSPARK_PACKING_GENESIS_3_3_BYTES;
					else if ($this->assetRef->txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX)
						$packing|=self::COINSPARK_PACKING_GENESIS_3_4_BYTES;
					else
						return null;

				} elseif ( ($this->assetRef->blockNum <= COINSPARK_UNSIGNED_4_BYTES_MAX) && ($this->assetRef->txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX) )
					$packing|=self::COINSPARK_PACKING_GENESIS_4_4_BYTES;
				
				else
					return null;
			}
				
		//	Packing for input and output indices
		
			$inputPackingOptions=$this->getPackingOptions(@$previousTransfer->inputs, $this->inputs, $countInputs);
			$outputPackingOptions=$this->getPackingOptions(@$previousTransfer->outputs, $this->outputs, $countOutputs);
			
			if ($inputPackingOptions['_0P'] && $outputPackingOptions['_0P'])
				$packing|=self::COINSPARK_PACKING_INDICES_0P_0P;
			elseif ($inputPackingOptions['_0P'] && $outputPackingOptions['_1S'])
				$packing|=self::COINSPARK_PACKING_INDICES_0P_1S;
			elseif ($inputPackingOptions['_0P'] && $outputPackingOptions['_ALL'])
				$packing|=self::COINSPARK_PACKING_INDICES_0P_ALL;
			elseif ($inputPackingOptions['_1S'] && $outputPackingOptions['_0P'])
				$packing|=self::COINSPARK_PACKING_INDICES_1S_0P;
			elseif ($inputPackingOptions['_ALL'] && $outputPackingOptions['_0P'])
				$packing|=self::COINSPARK_PACKING_INDICES_ALL_0P;
			elseif ($inputPackingOptions['_ALL'] && $outputPackingOptions['_1S'])
				$packing|=self::COINSPARK_PACKING_INDICES_ALL_1S;
			elseif ($inputPackingOptions['_ALL'] && $outputPackingOptions['_ALL'])
				$packing|=self::COINSPARK_PACKING_INDICES_ALL_ALL;
	
			else { // we need the second (extended) packing byte
				$packing|=self::COINSPARK_PACKING_INDICES_EXTEND;
		
				$packingExtendInput=$this->encodePackingExtend($inputPackingOptions);
				$packingExtendOutput=$this->encodePackingExtend($outputPackingOptions);

				if ( (!isset($packingExtendInput)) || (!isset($packingExtendOutput)) )
					return null;
					
				$packingExtend=($packingExtendInput << self::COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) | ($packingExtendOutput << self::COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT);
			}
			
		//	Packing for quantity
		
			$encodeQuantity=$this->qtyPerOutput;
		   
			if ($this->qtyPerOutput==(isset($previousTransfer) ? $previousTransfer->qtyPerOutput : 1))
				$packing|=self::COINSPARK_PACKING_QUANTITY_1P;
			else if ($this->qtyPerOutput>=COINSPARK_ASSET_QTY_MAX)
				$packing|=self::COINSPARK_PACKING_QUANTITY_MAX;
			else if ($this->qtyPerOutput<=COINSPARK_UNSIGNED_BYTE_MAX)
				$packing|=self::COINSPARK_PACKING_QUANTITY_1_BYTE;
			else if ($this->qtyPerOutput<=COINSPARK_UNSIGNED_2_BYTES_MAX)
				$packing|=self::COINSPARK_PACKING_QUANTITY_2_BYTES;
			else if ($this->qtyToMantissaExponent($this->qtyPerOutput, 0, self::COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX, 
					self::COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX, $qtyMantissa, $qtyExponent)==$this->qtyPerOutput) {
				$packing|=self::COINSPARK_PACKING_QUANTITY_FLOAT;
				$encodeQuantity=($qtyExponent*self::COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+$qtyMantissa)&self::COINSPARK_TRANSFER_QTY_FLOAT_MASK;
			} else if ($this->qtyPerOutput<=COINSPARK_UNSIGNED_3_BYTES_MAX)
				$packing|=self::COINSPARK_PACKING_QUANTITY_3_BYTES;
			else if ($this->qtyPerOutput<=COINSPARK_UNSIGNED_4_BYTES_MAX)
				$packing|=self::COINSPARK_PACKING_QUANTITY_4_BYTES;
			else
				$packing|=self::COINSPARK_PACKING_QUANTITY_6_BYTES;
				
		//	Write out the actual data
		
			$counts=$this->packingToByteCounts($packing, @$packingExtend);
			
			$metadata=chr($packing);
			
			if ( ($packing & self::COINSPARK_PACKING_INDICES_MASK) == self::COINSPARK_PACKING_INDICES_EXTEND)
				$metadata.=chr($packingExtend);
			
			$written_array=array(
				$this->writeUnsignedField($counts['blockNumBytes'], $this->assetRef->blockNum),
				$this->writeUnsignedField($counts['txOffsetBytes'], $this->assetRef->txOffset),
				substr(pack('H*', $this->assetRef->txIDPrefix).str_repeat("\x00", $counts['txIDPrefixBytes']), 0, $counts['txIDPrefixBytes']), // ensure right length
				$this->writeUnsignedField($counts['firstInputBytes'], $this->inputs->first),
				$this->writeUnsignedField($counts['countInputsBytes'], $this->inputs->count),
				$this->writeUnsignedField($counts['firstOutputBytes'], $this->outputs->first),
				$this->writeUnsignedField($counts['countOutputsBytes'], $this->outputs->count),
				$this->writeUnsignedField($counts['quantityBytes'], $encodeQuantity),
			);
			
			foreach ($written_array as $written)
				if (!isset($written))
					return null;
				else
					$metadata.=$written;
					
		//	Check the total length is within the specified limit
		
			if (strlen($metadata)>$metadataMaxLen)
				return null;
				
		//	Return what we created
		
			return $metadata;			
		}
		
		function decode($metadata, $previousTransfer, $countInputs, $countOutputs)
		{
			$startLength=strlen($metadata);
			
		//  Extract packing

			$packing=$this->shiftReadSmallEndianUnsigned($metadata, 1);
			if (!isset($packing))
				return 0;
				
		//  Packing for genesis reference
	
			switch ($packing & self::COINSPARK_PACKING_GENESIS_MASK)
			{
				case self::COINSPARK_PACKING_GENESIS_PREV:
					if (isset($previousTransfer))
						$this->assetRef=$previousTransfer->assetRef;
					
					else { // it's for a default route
						$this->assetRef->blockNum=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE;
						$this->assetRef->txOffset=0;
						$this->assetRef->txIDPrefix=str_repeat("00", COINSPARK_ASSETREF_TXID_PREFIX_LEN);
					}
					break;
			}
	
		//  Packing for input and output indices
	
			if (($packing & self::COINSPARK_PACKING_INDICES_MASK) == self::COINSPARK_PACKING_INDICES_EXTEND) { // we're using second packing metadata byte
				$packingExtend=$this->shiftReadSmallEndianUnsigned($metadata, 1);
				if (!isset($packingExtend))
					return 0;
					
				$inputPackingType=$this->decodePackingExtend(($packingExtend >> self::COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & self::COINSPARK_PACKING_EXTEND_MASK);
				$outputPackingType=$this->decodePackingExtend(($packingExtend >> self::COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & self::COINSPARK_PACKING_EXTEND_MASK);

				if ( (!isset($inputPackingType)) || (!isset($outputPackingType)) )
					return 0;
		
			} else { // not using second packing metadata byte

				switch ($packing & self::COINSPARK_PACKING_INDICES_MASK) // input packing
				{
					case self::COINSPARK_PACKING_INDICES_0P_0P:
					case self::COINSPARK_PACKING_INDICES_0P_1S:
					case self::COINSPARK_PACKING_INDICES_0P_ALL:
						$inputPackingType='_0P';
						break;
				
					case self::COINSPARK_PACKING_INDICES_1S_0P:
						$inputPackingType='_1S';
						break;
			
					case self::COINSPARK_PACKING_INDICES_ALL_0P:
					case self::COINSPARK_PACKING_INDICES_ALL_1S:
					case self::COINSPARK_PACKING_INDICES_ALL_ALL:
						$inputPackingType='_ALL';
						break;
				}
		
				switch ($packing & self::COINSPARK_PACKING_INDICES_MASK) // output packing
				{
					case self::COINSPARK_PACKING_INDICES_0P_0P:
					case self::COINSPARK_PACKING_INDICES_1S_0P:
					case self::COINSPARK_PACKING_INDICES_ALL_0P:
						$outputPackingType='_0P';
						break;
				
					case self::COINSPARK_PACKING_INDICES_0P_1S:
					case self::COINSPARK_PACKING_INDICES_ALL_1S:
						$outputPackingType='_1S';
						break;
				
					case self::COINSPARK_PACKING_INDICES_0P_ALL:
					case self::COINSPARK_PACKING_INDICES_ALL_ALL:
						$outputPackingType='_ALL';
						break;
				}
			}
	
		//  Final stage of packing for input and output indices
	
			$this->inputs=$this->packingTypeToValues($inputPackingType, @$previousTransfer->inputs, $countInputs);
			$this->outputs=$this->packingTypeToValues($outputPackingType, @$previousTransfer->outputs, $countOutputs);
	
		//  Read in the fields as appropriate
	
			$counts=$this->packingToByteCounts($packing, @$packingExtend);
			
			$txIDPrefixBytes=$counts['txIDPrefixBytes'];
			
			$read_array=array(
				$this->readUnsignedField($metadata, $counts['blockNumBytes'], $this->assetRef->blockNum),
				$this->readUnsignedField($metadata, $counts['txOffsetBytes'], $this->assetRef->txOffset),
				($txIDPrefixBytes==0) ? true : 
					(strlen($this->assetRef->txIDPrefix=strtoupper(bin2hex($this->stringShift($metadata, $txIDPrefixBytes))))==(2*$txIDPrefixBytes)),
				$this->readUnsignedField($metadata, $counts['firstInputBytes'], $this->inputs->first),
				$this->readUnsignedField($metadata, $counts['countInputsBytes'], $this->inputs->count),
				$this->readUnsignedField($metadata, $counts['firstOutputBytes'], $this->outputs->first),
				$this->readUnsignedField($metadata, $counts['countOutputsBytes'], $this->outputs->count),
				$this->readUnsignedField($metadata, $counts['quantityBytes'], $decodeQuantity),
			);

			foreach ($read_array as $read)
				if (!$read)
					return 0;
	
		//	Finish up reading in quantity
		
			switch ($packing & self::COINSPARK_PACKING_QUANTITY_MASK)
			{
				case self::COINSPARK_PACKING_QUANTITY_1P:
					if (isset($previousTransfer))
						$this->qtyPerOutput=$previousTransfer->qtyPerOutput;
					else
						$this->qtyPerOutput=1;
					break;
			
				case self::COINSPARK_PACKING_QUANTITY_MAX:
					$this->qtyPerOutput=COINSPARK_ASSET_QTY_MAX;
					break;
				
				case self::COINSPARK_PACKING_QUANTITY_FLOAT:
					$decodeQuantity&=self::COINSPARK_TRANSFER_QTY_FLOAT_MASK;
					$this->qtyPerOutput=$this->mantissaExponentToQty($decodeQuantity%self::COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE,
						(int)($decodeQuantity/self::COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE));
					break;
				
				default:
					$this->qtyPerOutput=$decodeQuantity;
					break;
			}
		
		//	Return bytes used
			
			if (!$this->isValid())
				return 0;
			
			return $startLength-strlen($metadata);
		}
		
		function toStringInner($headers)
		{
			$buffer=$headers ? "COINSPARK TRANSFER\n" : "";
			$isDefaultRoute=($this->assetRef->blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);
			
			if ($isDefaultRoute)
				$buffer.="      Default route:\n";
			
			else {
				$buffer.=$this->assetRef->toStringInner(false);
				$buffer.=sprintf("    Asset reference: %s\n", $this->assetRef->encode());
			}
			
			if ($this->inputs->count>0) {
				if ($this->inputs->count>1)
					$buffer.=sprintf("             Inputs: %d - %d (count %d)", $this->inputs->first,
						$this->inputs->first+$this->inputs->count-1, $this->inputs->count);
				else
					$buffer.=sprintf("              Input: %d", $this->inputs->first);
			} else
				$buffer.=sprintf("             Inputs: none");
		
			$buffer.=sprintf(" (small endian hex: first %s count %s)\n", $this->unsignedToSmallEndianHex($this->inputs->first, 2),
				$this->unsignedToSmallEndianHex($this->inputs->count, 2));
		
			if ($this->outputs->count>0) {
				if (($this->outputs->count>1) && !$isDefaultRoute)
					$buffer.=sprintf("            Outputs: %d - %d (count %d)", $this->outputs->first,
						$this->outputs->first+$this->outputs->count-1, $this->outputs->count);
				else
					$buffer.=sprintf("             Output: %d", $this->outputs->first);
			} else
				$buffer.=sprintf("            Outputs: none");
		
			$buffer.=sprintf(" (small endian hex: first %s count %s)\n", $this->unsignedToSmallEndianHex($this->outputs->first, 2),
				$this->unsignedToSmallEndianHex($this->outputs->count, 2));
			
			if (!$isDefaultRoute) {
				$buffer.=sprintf("     Qty per output: %.0f (small endian hex %s", $this->qtyPerOutput,
					$this->unsignedToSmallEndianHex($this->qtyPerOutput, 8));
					
				if ($this->qtyToMantissaExponent($this->qtyPerOutput, 0, self::COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX,
						self::COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX, $qtyMantissa, $qtyExponent)==$this->qtyPerOutput) {
					$encodeQuantity=($qtyExponent*self::COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+$qtyMantissa)&self::COINSPARK_TRANSFER_QTY_FLOAT_MASK;		
					$buffer.=sprintf(", as float %s", $this->unsignedToSmallEndianHex($encodeQuantity, self::COINSPARK_TRANSFER_QTY_FLOAT_LENGTH));
				}
				
				$buffer.=")\n";
			}
			
			if ($headers)
				$buffer.="END COINSPARK TRANSFER\n\n";
			
			return $buffer;
		}
		
		private function getPackingOptions($previousRange, $range, $countInputsOutputs)
		{
			$packingOptions=array();
			
			$firstZero=($range->first==0);
			$firstByte=($range->first<=COINSPARK_UNSIGNED_BYTE_MAX);
			$first2Bytes=($range->first<=COINSPARK_UNSIGNED_2_BYTES_MAX);
			$countOne=($range->count==1);
			$countByte=($range->count<=COINSPARK_UNSIGNED_BYTE_MAX);
			
			if (isset($previousRange)) {
				$packingOptions['_0P']=($range->first==$previousRange->first) && ($range->count==$previousRange->count);
				$packingOptions['_1S']=($range->first==($previousRange->first+$previousRange->count)) && $countOne;
				
			} else {
				$packingOptions['_0P']=$firstZero && $countOne;
				$packingOptions['_1S']=($range->first==1) && $countOne;
			}
			
			$packingOptions['_BYTE']=$firstByte && $countOne;
			$packingOptions['_2_BYTES']=$first2Bytes && $countOne;
			$packingOptions['_1_1_BYTES']=$firstByte && $countByte;
			$packingOptions['_2_1_BYTES']=$first2Bytes && $countByte;
			$packingOptions['_2_2_BYTES']=$first2Bytes && ($range->count<=COINSPARK_UNSIGNED_2_BYTES_MAX);
			$packingOptions['_ALL']=$firstZero && ($range->count>=$countInputsOutputs);
			
			return $packingOptions;
		}
		
		private function packingTypeToValues($packingType, $previousRange, $countInputOutputs)
		{
			$range=new CoinSparkIORange();
			
			switch ($packingType)
			{
				case '_0P':
					if (isset($previousRange)) {
						$range->first=$previousRange->first;
						$range->count=$previousRange->count;
					} else {
						$range->first=0;
						$range->count=1;
					}
					break;
			
				case '_1S':
					if (isset($previousRange))
						$range->first=$previousRange->first+$previousRange->count;
					else
						$range->first=1;
			
					$range->count=1;
					break;
			
				case '_BYTE':
				case '_2_BYTES':
					$range->count=1;
					break;
			
				case '_ALL':
					$range->first=0;
					$range->count=$countInputOutputs;
					break;
			}
			
			return $range;
		}

		private function packingToByteCounts($packing, $packingExtend)
		{
		
		//  Set default values for bytes for all fields to zero
		
			$counts=array(
				'blockNumBytes' => 0,
				'txOffsetBytes' => 0,
				'txIDPrefixBytes' => 0,
				
				'firstInputBytes' => 0,
				'countInputsBytes' => 0,
				'firstOutputBytes' => 0,
				'countOutputsBytes' => 0,
				
				'quantityBytes' => 0,
			);
			
		//	Packing for genesis reference
		
			switch ($packing & self::COINSPARK_PACKING_GENESIS_MASK)
			{
				case self::COINSPARK_PACKING_GENESIS_3_3_BYTES:
					$counts['blockNumBytes']=3;
					$counts['txOffsetBytes']=3;
					$counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
					break;
			
				case self::COINSPARK_PACKING_GENESIS_3_4_BYTES:
					$counts['blockNumBytes']=3;
					$counts['txOffsetBytes']=4;
					$counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
					break;
			
				case self::COINSPARK_PACKING_GENESIS_4_4_BYTES:
					$counts['blockNumBytes']=4;
					$counts['txOffsetBytes']=4;
					$counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN;
					break;
			}

		//  Packing for input and output indices (relevant for extended indices only)
	
			if (($packing & self::COINSPARK_PACKING_INDICES_MASK) == self::COINSPARK_PACKING_INDICES_EXTEND) {
	
			//  Input indices
		
				switch (($packingExtend >> self::COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & self::COINSPARK_PACKING_EXTEND_MASK)
				{
					case self::COINSPARK_PACKING_EXTEND_BYTE:
						$counts['firstInputBytes']=1;
						break;
				
					case self::COINSPARK_PACKING_EXTEND_2_BYTES:
						$counts['firstInputBytes']=2;
						break;
				
					case self::COINSPARK_PACKING_EXTEND_1_1_BYTES:
						$counts['firstInputBytes']=1;
						$counts['countInputsBytes']=1;
						break;
				
					case self::COINSPARK_PACKING_EXTEND_2_1_BYTES:
						$counts['firstInputBytes']=2;
						$counts['countInputsBytes']=1;
						break;
				
					case self::COINSPARK_PACKING_EXTEND_2_2_BYTES:
						$counts['firstInputBytes']=2;
						$counts['countInputsBytes']=2;
						break;
				}
		
			//  Output indices
		
				switch (($packingExtend >> self::COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & self::COINSPARK_PACKING_EXTEND_MASK)
				{
					case self::COINSPARK_PACKING_EXTEND_BYTE:
						$counts['firstOutputBytes']=1;
						break;
				
					case self::COINSPARK_PACKING_EXTEND_2_BYTES:
						$counts['firstOutputBytes']=2;
						break;
				
					case self::COINSPARK_PACKING_EXTEND_1_1_BYTES:
						$counts['firstOutputBytes']=1;
						$counts['countOutputsBytes']=1;
						break;
				
					case self::COINSPARK_PACKING_EXTEND_2_1_BYTES:
						$counts['firstOutputBytes']=2;
						$counts['countOutputsBytes']=1;
						break;
				
					case self::COINSPARK_PACKING_EXTEND_2_2_BYTES:
						$counts['firstOutputBytes']=2;
						$counts['countOutputsBytes']=2;
						break;
				}
		
			}
	
		//  Packing for quantity
	
			switch ($packing & self::COINSPARK_PACKING_QUANTITY_MASK)
			{
				case self::COINSPARK_PACKING_QUANTITY_1_BYTE:
					$counts['quantityBytes']=1;
					break;
			
				case self::COINSPARK_PACKING_QUANTITY_2_BYTES:
					$counts['quantityBytes']=2;
					break;
			
				case self::COINSPARK_PACKING_QUANTITY_3_BYTES:
					$counts['quantityBytes']=3;
					break;
			
				case self::COINSPARK_PACKING_QUANTITY_4_BYTES:
					$counts['quantityBytes']=4;
					break;
			
				case self::COINSPARK_PACKING_QUANTITY_6_BYTES:
					$counts['quantityBytes']=6;
					break;
			
				case self::COINSPARK_PACKING_QUANTITY_FLOAT:
					$counts['quantityBytes']=self::COINSPARK_TRANSFER_QTY_FLOAT_LENGTH;
					break;
			}
		
		//	Return the resulting array
			
			return $counts;
		}
		
		private function getPackingExtendMap()
		{
			 return array(
				'_0P' => self::COINSPARK_PACKING_EXTEND_0P,
				'_1S' => self::COINSPARK_PACKING_EXTEND_1S,
				'_ALL' => self::COINSPARK_PACKING_EXTEND_ALL,
				'_BYTE' => self::COINSPARK_PACKING_EXTEND_BYTE,
				'_2_BYTES' => self::COINSPARK_PACKING_EXTEND_2_BYTES,
				'_1_1_BYTES' => self::COINSPARK_PACKING_EXTEND_1_1_BYTES,
				'_2_1_BYTES' => self::COINSPARK_PACKING_EXTEND_2_1_BYTES,
				'_2_2_BYTES' => self::COINSPARK_PACKING_EXTEND_2_2_BYTES,
			); // in order of preference
		}
		
		private function encodePackingExtend($packingOptions)
		{
			$packingExtendMap=$this->getPackingExtendMap();
			
			foreach ($packingExtendMap as $packingType => $packingExtend)
				if ($packingOptions[$packingType])
					return $packingExtend;
					
			return null;
		}
		
		private function decodePackingExtend($packingExtend)
		{
			$packingExtendMap=$this->getPackingExtendMap();
			
			foreach ($packingExtendMap as $packingType => $testPackingExtend)
				if ($packingExtend==$testPackingExtend)
					return $packingType;
					
			return null;
		}

		private function writeUnsignedField($bytes, $source)
		{
			return ($bytes>0) ? $this->writeSmallEndianUnsigned($source, $bytes) : ''; // will return null on failure
		}
		
		private function readUnsignedField(&$metadata, $bytes, &$destination)
		{
			if ($bytes>0) {
				$value=$this->shiftReadSmallEndianUnsigned($metadata, $bytes);

				if (isset($value))
					$destination=$value;
				else
					return false;
			}
			
			return true;
		}
	}
	
	
//	CoinSparkTransferList class for managing list of asset transfer metadata
	
	class CoinSparkTransferList extends CoinSparkBase {
		public $transfers; // array of CoinSparkTransfer objects
		
		function __construct()
		{
			$this->clear();
		}
		
		function clear()
		{
			$this->transfers=array();
		}
		
		function toString()
		{
			$buffer="COINSPARK TRANSFERS\n";
		
			foreach ($this->transfers as $transferIndex => $transfer) {
				if ($transferIndex>0)
					$buffer.="\n";
				
				$buffer.=$transfer->toStringInner(false);
			}
		
			$buffer.="END COINSPARK TRANSFERS\n\n";
		
			return $buffer;
		}
		
		function isValid()
		{
			if (!is_array($this->transfers))
				return false;
			
			foreach ($this->transfers as $transfer)
				 if (!$transfer->isValid())
				 	return false;
				 	
			return true;
		}
		
		function match($otherTransfers, $strict)
		{
			$countTransfers=count($this->transfers);
			if ($countTransfers!=count($otherTransfers->transfers))
				return false;
			
			if ($strict) {
				foreach ($this->transfers as $transferIndex => $transfer)
					if (!$transfer->match($otherTransfers->transfers[$transferIndex]))
						return false;
			
			} else {
				$thisOrdering=$this->groupOrdering();
				$otherOrdering=$otherTransfers->groupOrdering();
				
				for ($transferIndex=0; $transferIndex<$countTransfers; $transferIndex++)
					if (!$this->transfers[$thisOrdering[$transferIndex]]->match($otherTransfers->transfers[$otherOrdering[$transferIndex]]))
						return false;
			}

			return true;
		}
		
		function encode($countInputs, $countOutputs, $metadataMaxLen)
		{

		//  4-character identifier
	
			$metadata=COINSPARK_METADATA_IDENTIFIER.COINSPARK_TRANSFERS_PREFIX;
		
		//	Encode each transfer, grouping by asset reference, but preserving original order otherwise
		
			$ordering=$this->groupOrdering();
			
			$countTransfers=count($this->transfers);
			$previousTransfer=null;
			
			for ($transferIndex=0; $transferIndex<$countTransfers; $transferIndex++) {
				$thisTransfer=$this->transfers[$ordering[$transferIndex]];
				
				$written=$thisTransfer->encode($previousTransfer, $metadataMaxLen-strlen($metadata), $countInputs, $countOutputs);
				if (!isset($written))
					return null;
					
				$metadata.=$written;
				$previousTransfer=$thisTransfer;
			}
		
		//	Extra length check (even though $thisTransfer->encode() should be sufficient)
		
			if (strlen($metadata)>$metadataMaxLen)
				return null;
				
		//	Return what we created
		
			return $metadata;			
		}
		
		function decode($metadata, $countInputs, $countOutputs)
		{
			$metadata=CoinSparkLocateMetadataRange($metadata, COINSPARK_TRANSFERS_PREFIX);
			if (!isset($metadata))
				return 0;
		
		//	Iterate over list
		
			$this->transfers=array();
			$previousTransfer=null;
		
			while (strlen($metadata)>0) {
				$transfer=new CoinSparkTransfer();
				$transferBytesUsed=$transfer->decode($metadata, $previousTransfer, $countInputs, $countOutputs);
				
				if ($transferBytesUsed>0) {
					$this->transfers[]=$transfer;
					$metadata=substr($metadata, $transferBytesUsed);
					$previousTransfer=$transfer;
				
				} else
					return 0; // something was invalid
			}
	
		//	Return count
	
			return count($this->transfers);
		}
		
		function calcMinFee($countInputs, $outputsSatoshis, $outputsRegular)
		{
			$countOutputs=count($outputsSatoshis);
			if ($countOutputs!=count($outputsRegular))
				return COINSPARK_SATOSHI_QTY_MAX; // these two arrays must be the same size
			
			$transfersToCover=0;
			
			foreach ($this->transfers as $transfer) {
				if (
					($transfer->assetRef->blockNum != COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) && // don't count default routes
					($transfer->inputs->count>0) &&
					($transfer->inputs->first<$countInputs) // only count if at least one valid input index
				) {
					$outputIndex=max($transfer->outputs->first, 0);
					$lastOutputIndex=min($transfer->outputs->first+$transfer->outputs->count, $countOutputs)-1;
			
					for (; $outputIndex<=$lastOutputIndex; $outputIndex++)
						if ($outputsRegular[$outputIndex])
							$transfersToCover++;
				}
			}
	
			return $transfersToCover*$this->getMinFeeBasis($outputsSatoshis, $outputsRegular);
		}
		
		function apply($assetRef, $genesis, $inputBalances, $outputsRegular)
		{
		
		//	Zero output quantities and get counts
			
			$countInputs=count($inputBalances);
			$countOutputs=count($outputsRegular);
			$outputBalances=array_fill(0, $countOutputs, 0);
			
		//  Perform explicit transfers (i.e. not default routes)
			
			foreach ($this->transfers as $transfer) {
				if ($assetRef->match($transfer->assetRef)) {
					$inputIndex=max($transfer->inputs->first, 0);
					$outputIndex=max($transfer->outputs->first, 0);
			
					$lastInputIndex=min($inputIndex+$transfer->inputs->count, $countInputs)-1;
					$lastOutputIndex=min($outputIndex+$transfer->outputs->count, $countOutputs)-1;
			
					for (; $outputIndex<=$lastOutputIndex; $outputIndex++)
						if ($outputsRegular[$outputIndex]) {
							$transferRemaining=$transfer->qtyPerOutput;
					
							while ($inputIndex<=$lastInputIndex) {
								$transferQuantity=min($transferRemaining, $inputBalances[$inputIndex]);
						
								if ($transferQuantity>0) { // skip all this if nothing is to be transferred (branch not really necessary)
									$inputBalances[$inputIndex]-=$transferQuantity;
									$transferRemaining-=$transferQuantity;
									$outputBalances[$outputIndex]+=$transferQuantity;
								}
						
								if ($transferRemaining>0)
									$inputIndex++; // move to next input since this one is drained
								else
									break; // stop if we have nothing left to transfer
							}
						}
				}
			}
			
		//	Apply payment charges to all quantities not routed by default
		
			for ($outputIndex=0; $outputIndex<$countOutputs; $outputIndex++)
				if ($outputsRegular[$outputIndex])
					$outputBalances[$outputIndex]=$genesis->calcNet($outputBalances[$outputIndex]);
					
		//	Send remaining quantities to default outputs
		
			$inputDefaultOutput=$this->getDefaultRouteMap($countInputs, $outputsRegular);
			
			foreach ($inputDefaultOutput as $inputIndex => $outputIndex)
				if (isset($outputIndex))
					$outputBalances[$outputIndex]+=$inputBalances[$inputIndex];
					
		//	Return the result
		
			return $outputBalances;
		}
		
		function applyNone($assetRef, $genesis, $inputBalances, $outputsRegular)
		{
			$countOutputs=count($outputsRegular);
			$outputBalances=array_fill(0, $countOutputs, 0);

			$outputIndex=$this->getLastRegularOutput($outputsRegular);
			if (isset($outputIndex))
				$outputBalances[$outputIndex]=array_sum($inputBalances);
				
			return $outputBalances;
		}
		
		function defaultOutputs($countInputs, $outputsRegular)
		{
			$outputsDefault=array_fill(0, count($outputsRegular), false);
			
			$inputDefaultOutput=$this->getDefaultRouteMap($countInputs, $outputsRegular);
			
			foreach ($inputDefaultOutput as $outputIndex)
				if (isset($outputIndex))
					$outputsDefault[$outputIndex]=true;
					
			return $outputsDefault;
		}
		
		private function groupOrdering()
		{
			$countTransfers=count($this->transfers);
			$transferUsed=array_fill(0, $countTransfers, false);
			$ordering=array();

			for ($orderIndex=0; $orderIndex<$countTransfers; $orderIndex++) {
				$bestTransferScore=0;
				$bestTransferIndex=-1;
				
				foreach ($this->transfers as $transferIndex => $transfer)
					if (!$transferUsed[$transferIndex]) {
						if ($transfer->assetRef->blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
							$transferScore=3; // top priority to default routes, which must be first in the encoded list
						else if (($orderIndex>0) && $transfer->assetRef->match($this->transfers[$ordering[$orderIndex-1]]->assetRef))
							$transferScore=2; // then next best is one which has same asset reference as previous
						else
							$transferScore=1; // otherwise any will do
							
						if ($transferScore>$bestTransferScore) { // if it's clearly the best, take it
							$bestTransferScore=$transferScore;
							$bestTransferIndex=$transferIndex;
							
						} else if ($transferScore==$bestTransferScore) // otherwise give priority to "lower" asset references
							if ($transfer->assetRef->compare($this->transfers[$bestTransferIndex]->assetRef)<0)
								$bestTransferIndex=$transferIndex;
					}
					
				$ordering[$orderIndex]=$bestTransferIndex;
				$transferUsed[$bestTransferIndex]=true;
			}
			
			return $ordering;
		}
		
		private function getDefaultRouteMap($countInputs, $outputsRegular)
		{
			$countOutputs=count($outputsRegular);
			
		//  Default to last output for all inputs
		
			$inputDefaultOutput=array_fill(0, $countInputs, $this->getLastRegularOutput($outputsRegular));
			
		//  Apply any default route transfers in reverse order (since early ones take precedence)

			$reverseTransfers=array_reverse($this->transfers);
			
			foreach ($reverseTransfers as $transfer)
				if ($transfer->assetRef->blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) {
					$outputIndex=$transfer->outputs->first;
					
					if (($outputIndex>=0) && ($outputIndex<$countOutputs)) {
						$inputIndex=max($transfer->inputs->first, 0);
						$lastInputIndex=min($inputIndex+$transfer->inputs->count, $countInputs)-1;
						
						for (; $inputIndex<=$lastInputIndex; $inputIndex++)
							$inputDefaultOutput[$inputIndex]=$outputIndex;
					}
				}
				
		//	Return the result
				
			return $inputDefaultOutput;
		}
	}
	

//	CoinSparkPaymentRef class for managing payment references

	class CoinSparkPaymentRef extends CoinSparkBase {
		public $ref; // the actual payment reference (number, can be float on 32-bit PHP)
		
		function __construct()
		{
			$this->clear();
		}
		
		function clear()
		{
			$this->ref=0;
		}
		
		function toString()
		{
			$buffer="COINSPARK PAYMENT REFERENCE\n";
			$buffer.=sprintf("%.0f (small endian hex %s)\n", $this->ref, $this->unsignedToSmallEndianHex($this->ref, 8));
			$buffer.="END COINSPARK PAYMENT REFERENCE\n\n";
			
			return $buffer;
		}
		
		function isValid()
		{
			return ($this->ref>=0) && ($this->ref<=COINSPARK_PAYMENT_REF_MAX) && $this->isInteger($this->ref);
		}
		
		function match($otherPaymentRef)
		{
			return $this->ref==$otherPaymentRef->ref;
		}
		
		function randomize()
		{
			$this->ref=0;
			
			for ($bitsRemaining=COINSPARK_PAYMENT_REF_MAX; $bitsRemaining>0; $bitsRemaining=floor($bitsRemaining/8192)) {
				$this->ref*=8192;
				$this->ref+=rand(0, 8191);
			}
			
			return $this->ref=round(fmod($this->ref, 1+COINSPARK_PAYMENT_REF_MAX));
		}
		
		function encode($metadataMaxLen)
		{
			if (!$this->isValid())
				return null;
				
		//	4-character identifier
				
			$metadata=COINSPARK_METADATA_IDENTIFIER.COINSPARK_PAYMENTREF_PREFIX;
		
		//	The payment reference
			
			$bytes=0;
			for ($paymentLeft=$this->ref; $paymentLeft>0; $paymentLeft=floor($paymentLeft/256))
				$bytes++;
				
			$metadata.=$this->writeSmallEndianUnsigned($this->ref, $bytes);
			
		//	Check the total length is within the specified limit
		
			if (strlen($metadata)>$metadataMaxLen)
				return null;
				
		//	Return what we created
		
			return $metadata;
		}
		
		function decode($metadata)
		{
			$metadata=CoinSparkLocateMetadataRange($metadata, COINSPARK_PAYMENTREF_PREFIX);
			if (!isset($metadata))
				return false;
				
		//	The payment reference
		
			$finalMetadataLen=strlen($metadata);
			if ($finalMetadataLen>8)
				return false;
		
			$this->ref=$this->readSmallEndianUnsigned($metadata, $finalMetadataLen);
				
		//	Return validity
		
			return $this->isValid();			
		}
	}
	
	
//	Class used internally for input or output ranges	

	class CoinSparkIORange extends CoinSparkBase {
		public $first; // integer
		public $count; // integer
		
		function __construct()
		{
			$this->clear();
		}
		
		function clear()
		{
			$this->first=0;
			$this->count=0;
		}
		
		function isValid()
		{
			if ( ($this->first<0) || ($this->first>COINSPARK_IO_INDEX_MAX) || !$this->isInteger($this->first) )
				return false;
				
			if ( ($this->count<0) || ($this->count>COINSPARK_IO_INDEX_MAX) || !$this->isInteger($this->count) )
				return false;
			
			return true;
		}
		
		function match($otherInOutRange)
		{
			return ($this->first==$otherInOutRange->first) && ($this->count==$otherInOutRange->count);
		}
	}
	

//	Base class implementing utility functions used internally

	class CoinSparkBase {
	
		protected function isInteger($value)
		{
			return is_numeric($value) && (intval($value)==$value); // allows numerical strings
		}
		
		protected function isBoolean($value)
		{
			return ($value===true) || ($value===false) || ($this->isInteger($value) && (($value==0) || ($value==1)));
				// allows anything that could reasonably be used to represent a true or false value
		}
		
		protected function isString($value)
		{
			return is_string($value) || is_numeric($value); // also allows integers and floats
		}

		protected function writeSmallEndianUnsigned($value, $bytes)
		{
			if ($value<0)
				return null; // does not support negative values
		
			$buffer='';

			for ($byte=0; $byte<$bytes; $byte++) {
				$buffer.=chr(round(fmod($value, 256)));
				$value=floor($value/256);
			}
		
			return $value ? null : $buffer; // if still something left, we didn't have enough bytes for representation
		}
	
		protected function readSmallEndianUnsigned($buffer, $bytes)
		{
			if (strlen($buffer)<$bytes)
				return null;

			$value=0;
		
			for ($byte=$bytes-1; $byte>=0; $byte--) {
				$value*=256;
				$value+=ord($buffer[$byte]);
			}
		
			return $value;
		}
	
		protected function stringShift(&$string, $chars)
		{
			$prefix=substr($string, 0, $chars);
			$string=substr($string, $chars);

			return $prefix;
		}
	
		protected function shiftReadSmallEndianUnsigned(&$string, $bytes)
		{
			return $this->readSmallEndianUnsigned($this->stringShift($string, $bytes), $bytes);
		}
	
		protected function unsignedToSmallEndianHex($value, $bytes)
		{
			$string='';
		
			for ($byte=0; $byte<$bytes; $byte++) {
				$string.=sprintf('%02X', round(fmod($value, 256)));
				$value=floor($value/256);
			}
	
			return $string;
		}
		
		protected function base58ToInteger($base58Character)
		{
			$integer=strpos(COINSPARK_INTEGER_TO_BASE_58, $base58Character);
		
			return ($integer===false) ? null : $integer;
		}
		
		protected function mantissaExponentToQty($mantissa, $exponent)
		{
			$quantity=$mantissa;
		
			for (; $exponent>0; $exponent--)
				$quantity*=10;
		
			return $quantity;
		}
	
		protected function qtyToMantissaExponent($quantity, $rounding, $mantissaMax, $exponentMax, &$mantissa, &$exponent)
		{
			if ($rounding<0)
				$roundOffset=0;
			elseif ($rounding>0)
				$roundOffset=9;
			else
				$roundOffset=4;
			
			$exponent=0;
		
			while ($quantity>$mantissaMax) {
				$quantity=floor(($quantity+$roundOffset)/10);
				$exponent++;
			}
		
			$mantissa=$quantity;
			$exponent=min($exponent, $exponentMax);
		
			return $this->mantissaExponentToQty($mantissa, $exponent);
		}
	
		protected function getMinFeeBasis($outputsSatoshis, $outputsRegular)
		{
			$smallestOutputSatoshis=COINSPARK_SATOSHI_QTY_MAX;
	
			if (count($outputsSatoshis)==count($outputsRegular)) // if arrays different size, we can't use them
				foreach ($outputsSatoshis as $outputIndex => $satoshis)
					if ($outputsRegular[$outputIndex])
						$smallestOutputSatoshis=min($smallestOutputSatoshis, $satoshis);

			return min(COINSPARK_FEE_BASIS_MAX_SATOSHIS, $smallestOutputSatoshis);
		}
	
		protected function getLastRegularOutput($outputsRegular)
		{
			$lastRegularOutput=null;
		
			foreach ($outputsRegular as $outputIndex => $isRegular)
				if ($isRegular)
					$lastRegularOutput=isset($lastRegularOutput) ? max($outputIndex, $lastRegularOutput) : $outputIndex;
				
			return $lastRegularOutput;
		}
		
		protected function countNonLastRegularOutputs($outputsRegular)
		{
			$countRegularOutputs=0;
			
			foreach ($outputsRegular as $outputRegular)
				if ($outputRegular)
					$countRegularOutputs++;
					
			return max($countRegularOutputs-1, 0);
		}
	
		protected function shrinkLowerDomainName($domainName, &$packing)
		{
			global $COINSPARK_DOMAIN_NAME_PREFIXES, $COINSPARK_DOMAIN_NAME_SUFFIXES; // quasi constants

			if (!strlen($domainName))
				return null;
			
			$domainName=strtolower($domainName);
	
		//	Search for prefixes
	
			$bestPrefixLen=-1;
			foreach ($COINSPARK_DOMAIN_NAME_PREFIXES as $prefixIndex => $prefix) {
				$prefixLen=strlen($prefix);
			
				if ( ($prefixLen>$bestPrefixLen) && (substr($domainName, 0, $prefixLen)==$prefix) ) {
					$bestPrefixIndex=$prefixIndex;
					$bestPrefixLen=$prefixLen;
				}
			}

			$domainName=substr($domainName, $bestPrefixLen);
			$domainNameLen=strlen($domainName);
		
		//	Search for suffixes
	
			$bestSuffixLen=-1;
			foreach ($COINSPARK_DOMAIN_NAME_SUFFIXES as $suffixIndex => $suffix) {
				$suffixLen=strlen($suffix);
			
				if ( ($suffixLen>$bestSuffixLen) && (substr($domainName, $domainNameLen-$suffixLen)==$suffix) ) {
					$bestSuffixIndex=$suffixIndex;
					$bestSuffixLen=$suffixLen;
				}
			}
		
			$domainName=substr($domainName, 0, $domainNameLen-$bestSuffixLen);
		
		//	Output and return
	
			$packing=(($bestPrefixIndex<<COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT)&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)|
				($bestSuffixIndex&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK);
		
			return $domainName;
		}
	
		protected function expandDomainName($domainName, $packing)
		{
			global $COINSPARK_DOMAIN_NAME_PREFIXES, $COINSPARK_DOMAIN_NAME_SUFFIXES; // quasi constants
		
		//	Prefix
	
			$prefixIndex=($packing&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)>>COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT;
			$prefix=@$COINSPARK_DOMAIN_NAME_PREFIXES[$prefixIndex];
			if (!isset($prefix))
				return null;
		
		//	Suffix
	
			$suffixIndex=$packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK;
			$suffix=@$COINSPARK_DOMAIN_NAME_SUFFIXES[$suffixIndex];
			if (!isset($suffix))
				return null;
		
			return $prefix.$domainName.$suffix;
		}
	
		protected function readIPv4Address($domainName)
		{
			if (long2ip(ip2long($domainName))==$domainName)
				return explode('.', $domainName);
			else
				return null;
		}
		
		protected function encodeDomainPathTriplets($string)
		{
			$stringLen=strlen($string);
			$metadata='';

			for ($stringPos=0; $stringPos<$stringLen; $stringPos++) {
				$encodeValue=strpos(COINSPARK_DOMAIN_PATH_CHARS, $string[$stringPos]);
				if ($encodeValue===false)
					return null;
		
				switch ($stringPos%3) {
					case 0:
						$stringTriplet=$encodeValue;
						break;
				
					case 1:
						$stringTriplet+=$encodeValue*COINSPARK_DOMAIN_PATH_ENCODE_BASE;
						break;
				
					case 2:
						$stringTriplet+=$encodeValue*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE;
						break;
				}
		
				if ( (($stringPos%3)==2) || ($stringPos==($stringLen-1)) ) { // write out 2 bytes if we've collected 3 chars, or if we're finishing
					$written=$this->writeSmallEndianUnsigned($stringTriplet, 2);
					if (!isset($written))
						return null;

					$metadata.=$written;
				}
			}
			
			return $metadata;
		}
		
		protected function shiftDecodeDomainPathTriplets(&$metadata, $parts)
		{
			$string='';
			$stringPos=0;
			$domainPathChars=COINSPARK_DOMAIN_PATH_CHARS;
			
			while ($parts>0) {

				if (($stringPos%3)==0) {
					$stringTriplet=$this->shiftReadSmallEndianUnsigned($metadata, 2);
					if (!isset($stringTriplet))
						return null;

					if ($stringTriplet>=(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE))
						return null; // invalid value
				}
		
				switch ($stringPos%3)
				{
					case 0:
						$decodeValue=$stringTriplet%COINSPARK_DOMAIN_PATH_ENCODE_BASE;
						break;
				
					case 1:
						$decodeValue=((int)($stringTriplet/COINSPARK_DOMAIN_PATH_ENCODE_BASE))%COINSPARK_DOMAIN_PATH_ENCODE_BASE;
						break;
				
					case 2:
						$decodeValue=(int)($stringTriplet/(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE));
						break;
				}
		
				$decodeChar=$domainPathChars[$decodeValue];
				$string.=$decodeChar;
				$stringPos++;
	   
				if (($decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR) || ($decodeChar==COINSPARK_DOMAIN_PATH_FALSE_END_CHAR))
					$parts--;
			}
	
			return $string;
		}
		
		protected function encodeDomainAndOrPath($domainName, $useHttps, $pagePath, $usePrefix)
		{
			$metadata='';
			$encodeString='';
			
		//  Domain name

			if (isset($domainName)) {
				$octets=$this->readIPv4Address($domainName);
		
				if (isset($octets)) {
					$metadata.=chr(COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4+($useHttps ? COINSPARK_DOMAIN_PACKING_IPv4_HTTPS : 0));
			
					$metadata.=chr($octets[0]);
					$metadata.=chr($octets[1]);
					$metadata.=chr($octets[2]);
					$metadata.=chr($octets[3]);
		
				} else {
					$encodeString.=$this->shrinkLowerDomainName($domainName, $packing);
					$encodeString.=$useHttps ? COINSPARK_DOMAIN_PATH_TRUE_END_CHAR : COINSPARK_DOMAIN_PATH_FALSE_END_CHAR;
					
					$metadata.=chr($packing);
				}
			}
			
		//	Page path
		
			if (isset($pagePath)) {
				$encodeString.=$pagePath;
				$encodeString.=$usePrefix ? COINSPARK_DOMAIN_PATH_TRUE_END_CHAR : COINSPARK_DOMAIN_PATH_FALSE_END_CHAR;
			}
			
		//	Encode whatever is required as triplets
		
			if (strlen($encodeString)) {
				$written=$this->encodeDomainPathTriplets($encodeString);
				if (!isset($written))
					return null;
					
				$metadata.=$written;
			}
			
			return $metadata;			
		}
	
		protected function shiftDecodeDomainAndOrPath(&$metadata, $doDomainName, $doPagePath)
		{
			$result=array();
			$metadataParts=0;

		//	Domain name
		
			if ($doDomainName) {
			
			//	Get packing byte
			
				$packingChar=$this->stringShift($metadata, 1);
				if (!strlen($packingChar))
					return null;
			
				$packing=ord($packingChar);
			
			//	Extract IP address if present
			 	
			 	$isIpAddress=(($packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK)==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4);
			 	
			 	if ($isIpAddress) {
					$result['useHttps']=($packing&COINSPARK_DOMAIN_PACKING_IPv4_HTTPS) ? true : false;
			
					$octetChars=$this->stringShift($metadata, 4);
					if (strlen($octetChars)!=4)
						return null;
				
					$result['domainName']=sprintf("%u.%u.%u.%u", ord($octetChars[0]), ord($octetChars[1]), ord($octetChars[2]), ord($octetChars[3]));

			 	} else
			 		$metadataParts++;
			}
			
		//	Convert remaining metadata to string
		
			if ($doPagePath)
				$metadataParts++;
	
			if ($metadataParts>0) {
				$decodeString=$this->shiftDecodeDomainPathTriplets($metadata, $metadataParts);
				if (!isset($decodeString))
					return null;

			//  Extract domain name if IP address was not present
		
				if ($doDomainName && !$isIpAddress) {
					$endCharPos=strpos(
						strtr($decodeString, COINSPARK_DOMAIN_PATH_FALSE_END_CHAR, COINSPARK_DOMAIN_PATH_TRUE_END_CHAR),
						COINSPARK_DOMAIN_PATH_TRUE_END_CHAR
					);
					
					if ($endCharPos===false)
						return null; // should never happen
						
					$result['domainName']=$this->expandDomainName(substr($decodeString, 0, $endCharPos), $packing);
					if (!isset($result['domainName']))
						return null;
						
					$result['useHttps']=($decodeString[$endCharPos]==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR);

					$decodeString=substr($decodeString, $endCharPos+1);
				}
		
			//  Extract page path
		
				if ($doPagePath) {
					$endCharPos=strpos(
						strtr($decodeString, COINSPARK_DOMAIN_PATH_FALSE_END_CHAR, COINSPARK_DOMAIN_PATH_TRUE_END_CHAR),
						COINSPARK_DOMAIN_PATH_TRUE_END_CHAR
					);

					if ($endCharPos===false)
						return null; // should never happen
						
					$result['pagePath']=substr($decodeString, 0, $endCharPos);
					$result['usePrefix']=($decodeString[$endCharPos]==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR);
					$decodeString=substr($decodeString, $endCharPos+1);
				}
			}
	
		//  Finish and return
	
			return $result;
		}
	}
	

//	Other functions used internally
	
	function CoinSparkGetRawScript($scriptPubKey, $scriptIsHex)
	{
		if ($scriptIsHex) {
			if ( (strlen($scriptPubKey)%2) || preg_match('/[^0-9A-Fa-f]/', $scriptPubKey) ) // check valid hex
				return null;
			else
				return pack('H*', $scriptPubKey);

		} else
			return $scriptPubKey;
	}
	
	function CoinSparkLocateMetadataRange($metadata, $desiredPrefix)
	{
		$metadataLen=strlen($metadata);
	
		if ($metadataLen<(COINSPARK_METADATA_IDENTIFIER_LEN+1)) // check for 4 bytes at least
			return null; 
		
		if (substr($metadata, 0, COINSPARK_METADATA_IDENTIFIER_LEN)!=COINSPARK_METADATA_IDENTIFIER) // check it starts 'SPK'
			return null; 
		
		$position=COINSPARK_METADATA_IDENTIFIER_LEN; // start after 'SPK'

		while ($position<$metadataLen) {
			$foundPrefix=substr($metadata, $position, 1); // read the next prefix
			$position++;
			$foundPrefixOrd=ord($foundPrefix);
		
			if ( isset($desiredPrefix) ? ($foundPrefix==$desiredPrefix) : ($foundPrefixOrd>COINSPARK_LENGTH_PREFIX_MAX) )
				// it's our data from here to the end (if $desiredPrefix is null, it matches the last one whichever it is)
				return substr($metadata, $position);
		
			if ($foundPrefixOrd>COINSPARK_LENGTH_PREFIX_MAX) // it's some other type of data from here to end
				return null;
			
			// if we get here it means we found a length byte
		
			if (($position+$foundPrefixOrd)>$metadataLen) // something went wrong - length indicated is longer than that available
				return null;
			
			if ($position>=$metadataLen) // something went wrong - that was the end of the input data
				return null; 
			
			if (substr($metadata, $position, 1)==$desiredPrefix) // it's the length of our part
				return substr($metadata, $position+1, $foundPrefixOrd-1);
			else
				$position+=$foundPrefixOrd; // skip over this many bytes
		}
	
		return null;
	}
		

//	... and a little bonus because it's easy in PHP

	function CoinSparkQueryAssetTrackingServer($url, $txn_outputs, $assets)
	{
		// $txn_outputs should be an array of array('txid' => [hex transaction id], 'vout' => [output index])
		// $assets should be an array of bitcoin txids of the transactions which created each asset of interest
		
		$request_id=time().'-'.rand(100000,999999);
		
		$request_txouts=array();
		foreach ($txn_outputs as $txn_output) // sanitize for request
			$request_txouts[]=array('txid' => $txn_output['txid'], 'vout' => $txn_output['vout']);
		
		$request=array(
			'id' => $request_id,
			'method' => 'coinspark_assets_get_qty',
			'params' => array(
				'assets' => array_values($colors),
				'txouts' => $request_txouts,
			),
		);
		
		$curl=curl_init($url);
		curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, 5);
		curl_setopt($curl, CURLOPT_TIMEOUT, 5);
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);	
		curl_setopt($curl, CURLOPT_POST, true);
		curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($request));
		$response_json=curl_exec($curl);
		curl_close($curl);
		
		return json_decode($response_json, true);
	}