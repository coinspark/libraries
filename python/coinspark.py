# CoinSpark 1.0 - Python library
#
# Copyright (c) 2014 Coin Sciences Ltd
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


import math, string, re, hashlib, binascii, random


# Quasi-constants for use by clients of the library

COINSPARK_SATOSHI_QTY_MAX = 2100000000000000
COINSPARK_ASSET_QTY_MAX = 100000000000000
COINSPARK_PAYMENT_REF_MAX = 4503599627370495 # 2^52-1

COINSPARK_GENESIS_QTY_MANTISSA_MIN = 1
COINSPARK_GENESIS_QTY_MANTISSA_MAX = 1000
COINSPARK_GENESIS_QTY_EXPONENT_MIN = 0
COINSPARK_GENESIS_QTY_EXPONENT_MAX = 11
COINSPARK_GENESIS_CHARGE_FLAT_MAX = 5000
COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN = 0
COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX = 100
COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX = 50
COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN = 0
COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX = 2
COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN = 0
COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX = 250
COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN = 32
COINSPARK_GENESIS_PAGE_PATH_MAX_LEN = 24
COINSPARK_GENESIS_HASH_MIN_LEN = 12
COINSPARK_GENESIS_HASH_MAX_LEN = 32

COINSPARK_ASSETREF_BLOCK_NUM_MAX = 4294967295
COINSPARK_ASSETREF_TX_OFFSET_MAX = 4294967295
COINSPARK_ASSETREF_TXID_PREFIX_LEN = 2

COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE = -1 # magic number for a default route

COINSPARK_IO_INDEX_MAX = 65535
	
COINSPARK_ADDRESS_FLAG_ASSETS = 1
COINSPARK_ADDRESS_FLAG_PAYMENT_REFS = 2
COINSPARK_ADDRESS_FLAG_MASK = 0x7FFFFF # 23 bits are currently usable


# Quasi-constants for internal use only
		
COINSPARK_UNSIGNED_BYTE_MAX = 0xFF
COINSPARK_UNSIGNED_2_BYTES_MAX = 0xFFFF
COINSPARK_UNSIGNED_3_BYTES_MAX = 0xFFFFFF
COINSPARK_UNSIGNED_4_BYTES_MAX = 4294967295

COINSPARK_METADATA_IDENTIFIER = "SPK"
COINSPARK_METADATA_IDENTIFIER_LEN = 3
COINSPARK_LENGTH_PREFIX_MAX = 96
COINSPARK_GENESIS_PREFIX = 'g'
COINSPARK_TRANSFERS_PREFIX = 't'
COINSPARK_PAYMENTREF_PREFIX = 'r'

COINSPARK_FEE_BASIS_MAX_SATOSHIS = 1000

COINSPARK_INTEGER_TO_BASE_58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"

COINSPARK_DOMAIN_PACKING_PREFIX_MASK = 0xC0
COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT = 6
COINSPARK_DOMAIN_PACKING_SUFFIX_MASK = 0x3F
COINSPARK_DOMAIN_PACKING_SUFFIX_MAX = 62
COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4 = 63
COINSPARK_DOMAIN_PACKING_IPv4_HTTPS = 0x40

COINSPARK_DOMAIN_PATH_ENCODE_BASE = 40
COINSPARK_DOMAIN_PATH_FALSE_END_CHAR = '<'
COINSPARK_DOMAIN_PATH_TRUE_END_CHAR= '>'
COINSPARK_DOMAIN_NAME_CHARS = "0123456789abcdefghijklmnopqrstuvwxyz-.<>"

COINSPARK_DOMAIN_NAME_PREFIXES=[
	"",
	"www."
]

COINSPARK_DOMAIN_NAME_SUFFIXES=[
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
]


# General public functions for managing CoinSpark metadata and bitcoin transaction output scripts

def CoinSparkScriptToMetadata(scriptPubKey, scriptIsHex):
	scriptPubKeyRaw=CoinSparkGetRawScript(scriptPubKey, scriptIsHex)
	scriptPubKeyRawLen=len(scriptPubKeyRaw)
	metadataLen=scriptPubKeyRawLen-2
	
	if (
		(scriptPubKeyRawLen>2) and
		(ord(scriptPubKeyRaw[0]) == 0x6a) and
		(ord(scriptPubKeyRaw[1]) > 0) and
		(ord(scriptPubKeyRaw[1]) <= 75) and
		(ord(scriptPubKeyRaw[1]) == metadataLen)
	):
		return scriptPubKeyRaw[2:]
		
	return None
	

def CoinSparkScriptsToMetadata(scriptPubKeys, scriptsAreHex):
	for scriptPubKey in scriptPubKeys:
		if not CoinSparkScriptIsRegular(scriptPubKey, scriptsAreHex):
			return CoinSparkScriptToMetadata(scriptPubKey, scriptsAreHex)
			
	return None
	

def CoinSparkMetadataToScript(metadata, toHexScript):
	if len(metadata)<=75:
		scriptPubKey=chr(0x6a)+chr(len(metadata))+metadata
		if toHexScript:
			scriptPubKey=CoinSparkRawStringToHex(scriptPubKey).upper()
		
		return scriptPubKey
		
	return None


def CoinSparkMetadataMaxAppendLen(metadata, metadataMaxLen):
	return max(metadataMaxLen-(len(metadata)+1-COINSPARK_METADATA_IDENTIFIER_LEN), 0)


def CoinSparkMetadataAppend(metadata, metadataMaxLen, appendMetadata):
	lastMetadata=CoinSparkLocateMetadataRange(metadata, None) # check we can find last metadata
	if lastMetadata is None:
		return None

	if len(appendMetadata)<(COINSPARK_METADATA_IDENTIFIER_LEN+1): # check there is enough to check the prefix
		return None
		
	if appendMetadata[:COINSPARK_METADATA_IDENTIFIER_LEN]!=COINSPARK_METADATA_IDENTIFIER: # then check the prefix
		return None
	
	# we don't check the character after the prefix in appendMetadata because it could itself be composite
	
	needLength=len(metadata)+len(appendMetadata)-COINSPARK_METADATA_IDENTIFIER_LEN+1 # check there is enough space
	if metadataMaxLen<needLength:
		return None
	
	lastMetadataLen=len(lastMetadata)+1 # include prefix
	lastMetadataPos=len(metadata)-lastMetadataLen
	
	return metadata[:lastMetadataPos]+chr(lastMetadataLen)+metadata[lastMetadataPos:]+appendMetadata[COINSPARK_METADATA_IDENTIFIER_LEN:]


def CoinSparkScriptIsRegular(scriptPubKey, scriptIsHex):
	scriptPubKeyRaw=CoinSparkGetRawScript(scriptPubKey, scriptIsHex)
			
	return (len(scriptPubKeyRaw)<1) or (ord(scriptPubKeyRaw[0])!=0x6a)
    

def CoinSparkCalcAssetHash(name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent):
	mask="\x09\x0A\x0D\x20"
	
	buffer=('' if name is None else str(name)).strip(mask)+"\x00"
	buffer+=('' if issuer is None else str(issuer)).strip(mask)+"\x00"
	buffer+=('' if description is None else str(description)).strip(mask)+"\x00"
	buffer+=('' if units is None else str(units)).strip(mask)+"\x00"
	buffer+=('' if issueDate is None else str(issueDate)).strip(mask)+"\x00"
	buffer+=('' if expiryDate is None else str(expiryDate)).strip(mask)+"\x00"
	
	interestRateToHash=math.floor((0 if interestRate is None else float(interestRate))*1000000.0+0.5)
	multipleToHash=math.floor((1 if multiple is None else float(multiple))*1000000.0+0.5)
	
	buffer+=("%.0f" % interestRateToHash)+"\x00"
	buffer+=("%.0f" % multipleToHash)+"\x00"
	
	buffer=buffer.encode('utf-8')
	
	if not contractContent is None:
		buffer+=contractContent
		
	buffer+="\x00".encode('utf-8') # to support Python 2.5 to 3+
	
	return hashlib.sha256(buffer).digest()


# Base class implementing utility functions used internally

class CoinSparkBase:
	COINSPARK_INTEGER_TO_BASE_58="123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
	
	def isInteger(self, value):
		return isinstance(value, int) or isinstance(value, long) or (isinstance(value, float) and (value%1==0))
		
		
	def isBoolean(self, value):
		return isinstance(value, bool) or (isinstance(value, int) and ((value==0) or (value==1)))


	def isString(self, value):
		return isinstance(value, str) or isinstance(value, int) or isinstance(value, long) or isinstance(value, float)
		
		
	def writeSmallEndianUnsigned(self, value, bytes):
		if value<0:
			return None # does not support negative values
	
		buffer=''
		
		for byte in range(bytes):
			buffer+=chr(int(round(value%256)))
			value=math.floor(value/256)
	
		return None if value else buffer # if still something left, we didn't have enough bytes for representation
	

	def readSmallEndianUnsigned(self, buffer, bytes):
		if len(buffer)<bytes:
			return None

		value=0
		
		for bufferChar in reversed(buffer[:bytes]):
			value*=256
			value+=ord(bufferChar)
	
		return value


	def unsignedToSmallEndianHex(self, value, bytes):
		string=''
		
		for byte in range(bytes):
			string+="%02X" % round(value%256)
			value=math.floor(value/256)
			
		return string
	

	def base58ToInteger(self, base58Character):
		integer=self.COINSPARK_INTEGER_TO_BASE_58.find(base58Character)
		
		return None if integer<0 else integer
		
		
	def mantissaExponentToQty(self, mantissa, exponent):
		quantity=mantissa
	
		while (exponent>0):
			quantity*=10
			exponent-=1
	
		return quantity
	

	def qtyToMantissaExponent(self, quantity, rounding, mantissaMax, exponentMax):
		if rounding<0:
			roundOffset=0
		elif rounding>0:
			roundOffset=9
		else:
			roundOffset=4
	
		exponent=0
	
		while quantity>mantissaMax:
			quantity=math.floor((quantity+roundOffset)/10)
			exponent+=1
		
		exponent=min(exponent, exponentMax)

		return {
			'mantissa': int(quantity),
			'exponent': int(exponent),
			'qty': self.mantissaExponentToQty(quantity, exponent)
		}


	def getMinFeeBasis(self, outputsSatoshis, outputsRegular):
		smallestOutputSatoshis=COINSPARK_SATOSHI_QTY_MAX

		if len(outputsSatoshis)==len(outputsRegular): # if arrays different size, we can't use them
			for outputIndex in range(len(outputsSatoshis)):
				if outputsRegular[outputIndex]:
					smallestOutputSatoshis=min(smallestOutputSatoshis, float(outputsSatoshis[outputIndex]))

		return min(COINSPARK_FEE_BASIS_MAX_SATOSHIS, smallestOutputSatoshis)


	def getLastRegularOutput(self, outputsRegular):
		lastRegularOutput=None
		
		for outputIndex in range(len(outputsRegular)):
			if outputsRegular[outputIndex]:
				lastRegularOutput=outputIndex if (lastRegularOutput is None) else max(outputIndex, lastRegularOutput)
			
		return lastRegularOutput


	def countNonLastRegularOutputs(self, outputsRegular):
		countRegularOutputs=0
		
		for outputRegular in outputsRegular:
			if (outputRegular):
				countRegularOutputs+=1
				
		return max(countRegularOutputs-1, 0)
	

	def shrinkLowerDomainName(self, domainName):
		if not len(domainName):
			return None
	
		domainName=domainName.lower()


		# Search for prefixes

		bestPrefixLen=-1
		
		for prefixIndex in range(len(COINSPARK_DOMAIN_NAME_PREFIXES)):
			prefix=COINSPARK_DOMAIN_NAME_PREFIXES[prefixIndex]
			prefixLen=len(prefix)
	
			if (prefixLen>bestPrefixLen) and (domainName[0:prefixLen]==prefix):
				bestPrefixIndex=prefixIndex
				bestPrefixLen=prefixLen


		domainName=domainName[bestPrefixLen:]
		domainNameLen=len(domainName)


		# Search for suffixes

		bestSuffixLen=-1
		
		for suffixIndex in range(len(COINSPARK_DOMAIN_NAME_SUFFIXES)):
			suffix=COINSPARK_DOMAIN_NAME_SUFFIXES[suffixIndex]
			suffixLen=len(suffix)
	
			if (suffixLen>bestSuffixLen) and (domainName[domainNameLen-suffixLen:]==suffix):
				bestSuffixIndex=suffixIndex
				bestSuffixLen=suffixLen


		domainName=domainName[:domainNameLen-bestSuffixLen]

		
		# Output and return

		packing=(((bestPrefixIndex<<COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT)&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)|
			(bestSuffixIndex&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK))

		return {
			'domainName': domainName,
			'packing': packing
		}


	def expandDomainName(self, domainName, packing):

		# Prefix

		prefixIndex=(packing&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)>>COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT
		if prefixIndex>len(COINSPARK_DOMAIN_NAME_PREFIXES):
			return None
			
		prefix=COINSPARK_DOMAIN_NAME_PREFIXES[prefixIndex]
	
		# Suffix

		suffixIndex=packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK
		if suffixIndex>len(COINSPARK_DOMAIN_NAME_SUFFIXES):
			return None
			
		suffix=COINSPARK_DOMAIN_NAME_SUFFIXES[suffixIndex]
	
		return prefix+domainName+suffix


	def readIPv4Address(self, domainName):
		if re.match('[^0-9\.]', domainName):
			return None
	
		octets=domainName.split('.')
		if len(octets)!=4:
			return None
		
		for octetIndex in range(4):
			if (len(octets[octetIndex])==0) or (int(octets[octetIndex])>255):
				return None
			
		return [int(octet) for octet in octets]


	def encodeDomainPathTriplets(self, string):
		stringLen=len(string)
		metadata=''

		for stringPos in range(stringLen):
			encodeValue=COINSPARK_DOMAIN_NAME_CHARS.find(string[stringPos])
			if encodeValue<0:
				return None
			
			stringPosMod3=stringPos%3
			
			if stringPosMod3==0:
				stringTriplet=encodeValue
				
			elif stringPosMod3==1:
				stringTriplet+=encodeValue*COINSPARK_DOMAIN_PATH_ENCODE_BASE
		
			elif stringPosMod3==2:
				stringTriplet+=encodeValue*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE


			if (stringPosMod3==2) or (stringPos==(stringLen-1)): # write out 2 bytes if we've collected 3 chars, or if we're finishing
				written=self.writeSmallEndianUnsigned(stringTriplet, 2)
				if written is None:
					return None

				metadata+=written

		return metadata


	def decodeDomainPathTriplets(self, metadata, parts):
		startLength=len(metadata)
		string=''
		stringPos=0
	
		while parts>0:

			if (stringPos%3)==0:
				stringTriplet=self.readSmallEndianUnsigned(metadata, 2)
				metadata=metadata[2:]
				if stringTriplet is None:
					return None

				if stringTriplet>=(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE):
					return None # invalid value


			stringPosMod3=stringPos%3
			
			if stringPosMod3==0:
				decodeValue=stringTriplet%COINSPARK_DOMAIN_PATH_ENCODE_BASE
		
			elif stringPosMod3==1:
				decodeValue=math.floor(stringTriplet/COINSPARK_DOMAIN_PATH_ENCODE_BASE)%COINSPARK_DOMAIN_PATH_ENCODE_BASE
				
			elif stringPosMod3==2:
				decodeValue=math.floor(stringTriplet/(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE))


			decodeChar=COINSPARK_DOMAIN_NAME_CHARS[int(decodeValue)]
			string=string+decodeChar
			stringPos+=1

			if (decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR) or (decodeChar==COINSPARK_DOMAIN_PATH_FALSE_END_CHAR):
				parts-=1


		return {'string':string, 'decodedChars':startLength-len(metadata)}


	def encodeDomainAndOrPath(self, domainName, useHttps, pagePath, usePrefix):
		metadata=''
		encodeString=''
	

		# Domain name

		if not domainName is None:
			octets=self.readIPv4Address(domainName)

			if not octets is None:
				metadata=metadata+(
					chr(COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4+(COINSPARK_DOMAIN_PACKING_IPv4_HTTPS if useHttps else 0))+
					chr(octets[0])+
					chr(octets[1])+
					chr(octets[2])+
					chr(octets[3])
				)

			else:
				result=self.shrinkLowerDomainName(domainName)

				encodeString+=result['domainName']
				encodeString+=COINSPARK_DOMAIN_PATH_TRUE_END_CHAR if useHttps else COINSPARK_DOMAIN_PATH_FALSE_END_CHAR
			
				metadata+=chr(result['packing'])
	
		# Page path

		if not pagePath is None:
			encodeString+=pagePath
			encodeString+=COINSPARK_DOMAIN_PATH_TRUE_END_CHAR if usePrefix else COINSPARK_DOMAIN_PATH_FALSE_END_CHAR

	
		# Encode whatever is required as triplets

		if len(encodeString)>0:
			written=self.encodeDomainPathTriplets(encodeString)
			if written is None:
				return None
				
			metadata+=written

	
		return metadata			


	def decodeDomainAndOrPath(self, metadata, doDomainName, doPagePath):
		startLength=len(metadata)
		result={}
		metadataParts=0

		# Domain name

		if doDomainName:
	
			# Get packing byte
	
			packingChar=metadata[0]
			metadata=metadata[1:]
			if len(packingChar)<1:
				return None
			
			packing=ord(packingChar)
	

			# Extract IP address if present
		
			isIpAddress=((packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK)==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4)
		
			if isIpAddress:
				result['useHttps']=True if (packing&COINSPARK_DOMAIN_PACKING_IPv4_HTTPS) else False
	
				octetChars=metadata[:4]
				metadata=metadata[4:]
				if len(octetChars)!=4:
					return None
		
				result['domainName']="%u.%u.%u.%u" % (ord(octetChars[0]), ord(octetChars[1]), ord(octetChars[2]), ord(octetChars[3]))

			else:
				metadataParts+=1

	
		# Convert remaining metadata to string

		if doPagePath:
			metadataParts+=1

		if metadataParts>0:
			decodeResult=self.decodeDomainPathTriplets(metadata, metadataParts)
			if decodeResult is None:
				return None
			metadata=metadata[decodeResult['decodedChars']:]
			decodeString=decodeResult['string']


			# Extract domain name if IP address was not present
		
			if doDomainName and not isIpAddress:
				endCharPos=(decodeString
					.replace(COINSPARK_DOMAIN_PATH_FALSE_END_CHAR, COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
					.find(COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
				)

				if endCharPos<0:
					return None # should never happen
			
				result['domainName']=self.expandDomainName(decodeString[0:endCharPos], packing)
				if result['domainName'] is None:
					return None
				
				result['useHttps']=(decodeString[endCharPos]==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)

				decodeString=decodeString[endCharPos+1:]


			# Extract page path

			if doPagePath:
				endCharPos=(decodeString
					.replace(COINSPARK_DOMAIN_PATH_FALSE_END_CHAR, COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
					.find(COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
				)

				if endCharPos<0:
					return None # should never happen
				
				result['pagePath']=decodeString[0:endCharPos]
				result['usePrefix']=(decodeString[endCharPos]==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
				decodeString=decodeString[endCharPos+1:]

		# Finish and return
		
		result['decodedChars']=startLength-len(metadata)

		return result


# CoinSparkAddress class for managing CoinSpark addresseses

class CoinSparkAddress(CoinSparkBase):
	COINSPARK_ADDRESS_PREFIX='s'
	COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE=10
	COINSPARK_ADDRESS_CHAR_INCREMENT=13
	
	def __init__(self):
		self.clear()

		
	def clear(self):
		self.bitcoinAddress=''
		self.addressFlags=0
		self.paymentRef=CoinSparkPaymentRef()

		
	def toString(self):
		flagsToStrings={
			COINSPARK_ADDRESS_FLAG_ASSETS: "assets",
			COINSPARK_ADDRESS_FLAG_PAYMENT_REFS: "payment references"
		}
		
		buffer="COINSPARK ADDRESS\n"
		buffer+="  Bitcoin address: %s\n" % self.bitcoinAddress
		buffer+="    Address flags: %d" % self.addressFlags
		
		flagOutput=False
		
		for flag, string in flagsToStrings.items():
			if self.addressFlags & flag:
				buffer+=(", " if flagOutput else " [")+string
				flagOutput=True
				
		buffer+=("]" if flagOutput else "")+"\n"
				
		buffer+="Payment reference: %.0f\n" % self.paymentRef.ref
		buffer+="END COINSPARK ADDRESS\n\n"
		
		return buffer
		

	def isValid(self):
		if (not self.isString(self.bitcoinAddress)) or (len(self.bitcoinAddress)==0):
			return False
			
		if (not self.isInteger(self.addressFlags)) or ((self.addressFlags&COINSPARK_ADDRESS_FLAG_MASK)!=self.addressFlags):
			return False
		
		return self.paymentRef.isValid()
		

	def match(self, otherAddress):
		return (str(self.bitcoinAddress)==str(otherAddress.bitcoinAddress) and
			self.addressFlags==otherAddress.addressFlags and
			self.paymentRef.match(otherAddress.paymentRef))
		

	def encode(self):
		if not self.isValid():
			return None
			
		stringBase58=[0, 0]
		
		# Build up extra data for address flags
		
		addressFlagChars=0
		testAddressFlags=self.addressFlags
		
		while testAddressFlags>0:
			stringBase58.append(int(testAddressFlags%58))
			testAddressFlags=int(testAddressFlags/58) # treat as an integer
			addressFlagChars+=1
			
		# Build up extra data for payment reference
		
		paymentRefChars=0
		testPaymentRef=self.paymentRef.ref
		
		while testPaymentRef>0:
			stringBase58.append(int(round(testPaymentRef%58)))
			testPaymentRef=math.floor(testPaymentRef/58)
			paymentRefChars+=1
			
		# Calculate and encode extra length
		
		extraDataChars=addressFlagChars+paymentRefChars
		bitcoinAddressLen=len(self.bitcoinAddress)
		stringLen=bitcoinAddressLen+2+extraDataChars
		
		stringBase58[1]=addressFlagChars*self.COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE+paymentRefChars
		
		# Convert the bitcoin address
		
		for charIndex in range(bitcoinAddressLen):
			charValue=self.base58ToInteger(self.bitcoinAddress[charIndex])
			if charValue is None:
				return None # invalid base58 character
				
			charValue+=self.COINSPARK_ADDRESS_CHAR_INCREMENT
			
			if extraDataChars>0:
				charValue+=stringBase58[2+charIndex%extraDataChars]
				
			stringBase58.append(charValue%58)
			
		# Obfuscate first half of address using second half to prevent common prefixes
		
		halfLength=int(math.ceil(stringLen/2.0))
		for charIndex in range(1, halfLength): # exclude first character
			stringBase58[charIndex]=(stringBase58[charIndex]+stringBase58[stringLen-charIndex])%58
			
		# Convert to base 58 and add prefix
		
		string=self.COINSPARK_ADDRESS_PREFIX
		for charValue in stringBase58[1:stringLen]:
			string+=self.COINSPARK_INTEGER_TO_BASE_58[charValue]
			
		return string
		

	def decode(self, string):
	
		# Check for basic validity
		
		stringLen=len(string)
		if stringLen<2:
			return False
			
		if string[0]!=self.COINSPARK_ADDRESS_PREFIX:
			return False
			
		# Convert from base 58
	
		stringBase58=[0]
		for stringChar in string[1:]: # exclude first character
			charValue=self.base58ToInteger(stringChar)
			if charValue is None:
				return False
			stringBase58.append(charValue)
			
		# De-obfuscate first half of address using second half
	
		halfLength=int(math.ceil(stringLen/2.0))
		for charIndex in range(1, halfLength): # exclude first character
			stringBase58[charIndex]=(stringBase58[charIndex]+58-stringBase58[stringLen-charIndex])%58
			
	
		# Get length of extra data
	
		charValue=stringBase58[1]
		addressFlagChars=int(charValue/self.COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE)
		paymentRefChars=charValue%self.COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE
		extraDataChars=addressFlagChars+paymentRefChars
		
		if stringLen<(2+extraDataChars):
			return False
			
		bitcoinAddressLen=stringLen-2-extraDataChars
		
		
		# Read the extra data for address flags
	
		self.addressFlags=0
		multiplier=1
		
		for charValue in stringBase58[2:2+addressFlagChars]:
			self.addressFlags+=charValue*multiplier
			multiplier*=58
			
	
		# Read the extra data for payment reference
	
		self.paymentRef.ref=0
		multiplier=1
		
		for charValue in stringBase58[2+addressFlagChars:2+extraDataChars]:
			self.paymentRef.ref+=charValue*multiplier
			multiplier*=58
			
			
		# Convert the bitcoin address
		
		self.bitcoinAddress=''
		
		for charIndex in range(bitcoinAddressLen):
			charValue=stringBase58[2+extraDataChars+charIndex]
			charValue+=58*2-self.COINSPARK_ADDRESS_CHAR_INCREMENT # avoid worrying about the result of modulo on negative numbers
			
			if extraDataChars>0:
				charValue-=stringBase58[2+charIndex%extraDataChars]
				
			self.bitcoinAddress+=self.COINSPARK_INTEGER_TO_BASE_58[charValue%58]
		
		
		return self.isValid()
		

# CoinSparkGenesis class for managing asset genesis metadata

class CoinSparkGenesis(CoinSparkBase):
	COINSPARK_GENESIS_QTY_FLAGS_LENGTH=2
	COINSPARK_GENESIS_QTY_MASK=0x3FFF
	COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE=1001
	COINSPARK_GENESIS_FLAG_CHARGE_FLAT=0x4000
	COINSPARK_GENESIS_FLAG_CHARGE_BPS=0x8000
	COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE=101
	COINSPARK_GENESIS_CHARGE_FLAT_LENGTH=1
	COINSPARK_GENESIS_CHARGE_BPS_LENGTH=1
	
	def __init__(self):
		self.clear()
		

	def clear(self):
		self.qtyMantissa=0
		self.qtyExponent=0
		self.chargeFlatMantissa=0
		self.chargeFlatExponent=0
		self.chargeBasisPoints=0
		self.useHttps=False
		self.domainName=''
		self.usePrefix=True
		self.pagePath=''
		self.assetHash=''
		self.assetHashLen=0
		
		
	def toString(self):
		quantity=self.getQty()
		quantityEncoded=(self.qtyExponent*self.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+self.qtyMantissa)&self.COINSPARK_GENESIS_QTY_MASK
		chargeFlat=self.getChargeFlat()
		chargeFlatEncoded=self.chargeFlatExponent*self.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+self.chargeFlatMantissa
		domainPathMetadata=self.encodeDomainAndOrPath(self.domainName, self.useHttps, self.pagePath, self.usePrefix)
		
		buffer="COINSPARK GENESIS\n"
		buffer+="   Quantity mantissa: %d\n" % self.qtyMantissa
		buffer+="   Quantity exponent: %d\n" % self.qtyExponent
		buffer+="    Quantity encoded: %d (small endian hex %s)\n" % (quantityEncoded,
			self.unsignedToSmallEndianHex(quantityEncoded, self.COINSPARK_GENESIS_QTY_FLAGS_LENGTH) )
		buffer+="      Quantity value: %.0f\n" % quantity
		buffer+="Flat charge mantissa: %d\n" % self.chargeFlatMantissa
		buffer+="Flat charge exponent: %d\n" % self.chargeFlatExponent
		buffer+=" Flat charge encoded: %d (small endian hex %s)\n" % (chargeFlatEncoded,
			self.unsignedToSmallEndianHex(chargeFlatEncoded, self.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH))
		buffer+="   Flat charge value: %.0f\n" % chargeFlat
		buffer+=" Basis points charge: %d (hex %s)\n" % (self.chargeBasisPoints,
			self.unsignedToSmallEndianHex(self.chargeBasisPoints, self.COINSPARK_GENESIS_CHARGE_BPS_LENGTH))
		buffer+="           Asset URL: %s://%s/%s%s/ (length %d+%d encoded %s length %d)\n" % (
			'https' if self.useHttps else 'http', self.domainName,
			"coinspark/" if self.usePrefix else "", self.pagePath if len(self.pagePath) else "[spent-txid]",
			len(self.domainName), len(self.pagePath),
			CoinSparkRawStringToHex(domainPathMetadata), len(domainPathMetadata)
		)
		buffer+="          Asset hash: %s (length %d)\n" % (CoinSparkRawStringToHex(self.assetHash[:self.assetHashLen]), self.assetHashLen)
		buffer+="END COINSPARK GENESIS\n\n"
		
		return buffer
		
	
	def isValid(self):
		if not (
			self.isInteger(self.qtyMantissa) and
			self.isInteger(self.qtyExponent) and
			self.isInteger(self.chargeFlatMantissa) and
			self.isInteger(self.chargeFlatExponent) and
			self.isInteger(self.chargeBasisPoints) and
			self.isBoolean(self.useHttps) and
			self.isString(self.domainName) and
			self.isBoolean(self.usePrefix) and
			self.isString(self.pagePath) and
			self.isString(self.assetHash) and
			self.isInteger(self.assetHashLen)
		):
			return False
		
		if (self.qtyMantissa<COINSPARK_GENESIS_QTY_MANTISSA_MIN) or (self.qtyMantissa>COINSPARK_GENESIS_QTY_MANTISSA_MAX):
			return False
			
		if (self.qtyExponent<COINSPARK_GENESIS_QTY_EXPONENT_MIN) or (self.qtyExponent>COINSPARK_GENESIS_QTY_EXPONENT_MAX):
			return False
		
		if (self.chargeFlatExponent<COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN) or (self.chargeFlatExponent>COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX):
			return False
			
		if self.chargeFlatMantissa<COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN:
			return False
			
		if self.chargeFlatMantissa > (COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX if (self.chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) else COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX):
			return False
			
		if (self.chargeBasisPoints<COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN) or (self.chargeBasisPoints>COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX):
			return False
			
		if len(self.domainName)>COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN:
			return False
			
		if len(self.pagePath)>COINSPARK_GENESIS_PAGE_PATH_MAX_LEN:
			return False
			
		if len(self.assetHash)<self.assetHashLen: # check we have at least as much data as specified by self.assetHashLen
			return False 
		
		if (self.assetHashLen<COINSPARK_GENESIS_HASH_MIN_LEN) or (self.assetHashLen>COINSPARK_GENESIS_HASH_MAX_LEN):
			return False
			
		return True

	
	def match(self, otherGenesis, strict):	
		hashCompareLen=min(self.assetHashLen, otherGenesis.assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN)
		
		if strict:
			floatQuantitiesMatch=(
				(self.qtyMantissa==otherGenesis.qtyMantissa) and (self.qtyExponent==otherGenesis.qtyExponent) and
				(self.chargeFlatMantissa==otherGenesis.chargeFlatMantissa) and (self.chargeFlatExponent==otherGenesis.chargeFlatExponent)
			)
		else:
			floatQuantitiesMatch=(self.getQty()==otherGenesis.getQty()) and (self.getChargeFlat()==otherGenesis.getChargeFlat())
		
		return (
			floatQuantitiesMatch and (self.chargeBasisPoints==otherGenesis.chargeBasisPoints) and
			(self.useHttps==otherGenesis.useHttps) and
			(self.domainName.lower()==otherGenesis.domainName.lower()) and
			(self.usePrefix==otherGenesis.usePrefix) and
			(self.pagePath.lower()==otherGenesis.pagePath.lower()) and
			(self.assetHash[:hashCompareLen].lower()==otherGenesis.assetHash[:hashCompareLen].lower())
		)

	
	def getQty(self):
		return self.mantissaExponentToQty(self.qtyMantissa, self.qtyExponent)

	
	def setQty(self, desiredQty, rounding):
		result=self.qtyToMantissaExponent(desiredQty, rounding, COINSPARK_GENESIS_QTY_MANTISSA_MAX,
			COINSPARK_GENESIS_QTY_EXPONENT_MAX)
		self.qtyMantissa=result['mantissa']
		self.qtyExponent=result['exponent']
			
		return self.getQty()

	
	def getChargeFlat(self):
		return self.mantissaExponentToQty(self.chargeFlatMantissa, self.chargeFlatExponent)

	
	def setChargeFlat(self, desiredChargeFlat, rounding):
		result=self.qtyToMantissaExponent(desiredChargeFlat, rounding, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX,
			COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX)
		self.chargeFlatMantissa=result['mantissa']
		self.chargeFlatExponent=result['exponent']
			
		if self.chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX:
			self.chargeFlatMantissa=min(self.chargeFlatMantissa, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX)
			
		return self.getChargeFlat()

	
	def calcCharge(self, qtyGross):
		charge=self.getChargeFlat()+math.floor((qtyGross*self.chargeBasisPoints+5000)/10000) # rounds to nearest
		
		return min(qtyGross, charge)

	
	def calcNet(self, qtyGross):
		return qtyGross-self.calcCharge(qtyGross)

	
	def calcGross(self, qtyNet):
		qtyNet=float(qtyNet)
		
		if qtyNet<=0:
			return 0 # no point getting past charges if we end up with zero anyway
			
		lowerGross=math.floor(((qtyNet+self.getChargeFlat())*10000)/(10000-self.chargeBasisPoints)) # divides rounding down
		
		return lowerGross if (self.calcNet(lowerGross)>=qtyNet) else (lowerGross+1)

	
	def calcHashLen(self, metadataMaxLen):
		assetHashLen=metadataMaxLen-COINSPARK_METADATA_IDENTIFIER_LEN-1-self.COINSPARK_GENESIS_QTY_FLAGS_LENGTH
		
		if self.chargeFlatMantissa>0:
			assetHashLen-=self.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH
			
		if self.chargeBasisPoints>0:
			assetHashLen-=self.COINSPARK_GENESIS_CHARGE_BPS_LENGTH
		
		domainPathLen=len(self.pagePath)+1
			
		if self.readIPv4Address(self.domainName):
			assetHashLen-=5 # packing and IP octets
		else:
			assetHashLen-=1 # packing
			domainPathLen+=len(self.shrinkLowerDomainName(self.domainName)['domainName'])+1
		
		assetHashLen-=2*int((domainPathLen+2)/3) # uses integer arithmetic
		
		return min(assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN)
	

	def encode(self, metadataMaxLen):
		if not self.isValid():
			return None

		# 4-character identifier
			
		metadata=COINSPARK_METADATA_IDENTIFIER+COINSPARK_GENESIS_PREFIX

		# Quantity mantissa and exponent
	
		quantityEncoded=(self.qtyExponent*self.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+self.qtyMantissa)&self.COINSPARK_GENESIS_QTY_MASK
		if self.chargeFlatMantissa>0:
			quantityEncoded|=self.COINSPARK_GENESIS_FLAG_CHARGE_FLAT
		if self.chargeBasisPoints>0:
			quantityEncoded|=self.COINSPARK_GENESIS_FLAG_CHARGE_BPS

		written=self.writeSmallEndianUnsigned(quantityEncoded, self.COINSPARK_GENESIS_QTY_FLAGS_LENGTH)
		if written is None:
			return None
		
		metadata+=written
		
		# Charges - flat and basis points
	
		if quantityEncoded & self.COINSPARK_GENESIS_FLAG_CHARGE_FLAT:
			chargeEncoded=self.chargeFlatExponent*self.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+self.chargeFlatMantissa
		
			written=self.writeSmallEndianUnsigned(chargeEncoded, self.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH)
			if written is None:
				return None
				
			metadata+=written

		if quantityEncoded & self.COINSPARK_GENESIS_FLAG_CHARGE_BPS:
			written=self.writeSmallEndianUnsigned(self.chargeBasisPoints, self.COINSPARK_GENESIS_CHARGE_BPS_LENGTH)
			if written is None:
				return None
				
			metadata+=written
		
		# Domain name and page path
	
		written=self.encodeDomainAndOrPath(self.domainName, self.useHttps, self.pagePath, self.usePrefix)
		if written is None:
			return None

		metadata+=written

		# Asset hash
	
		metadata+=self.assetHash[:self.assetHashLen]
		
		# Check the total length is within the specified limit
	
		if len(metadata)>metadataMaxLen:
			return None
			
		# Return what we created
	
		return metadata

	
	def decode(self, metadata):
		metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_GENESIS_PREFIX)
		if metadata is None:
			return False
		
		# Quantity mantissa and exponent
	
		quantityEncoded=self.readSmallEndianUnsigned(metadata, self.COINSPARK_GENESIS_QTY_FLAGS_LENGTH)
		metadata=metadata[self.COINSPARK_GENESIS_QTY_FLAGS_LENGTH:]
		if quantityEncoded is None:
			return False
			
		self.qtyMantissa=(quantityEncoded&self.COINSPARK_GENESIS_QTY_MASK)%self.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE
		self.qtyExponent=int((quantityEncoded&self.COINSPARK_GENESIS_QTY_MASK)/self.COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE)
	
		# Charges - flat and basis points
	
		if quantityEncoded & self.COINSPARK_GENESIS_FLAG_CHARGE_FLAT:
			chargeEncoded=self.readSmallEndianUnsigned(metadata, self.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH)
			metadata=metadata[self.COINSPARK_GENESIS_CHARGE_FLAT_LENGTH:]
			
			self.chargeFlatMantissa=chargeEncoded%self.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE
			self.chargeFlatExponent=int(chargeEncoded/self.COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE)
		
		else:
			self.chargeFlatMantissa=0
			self.chargeFlatExponent=0
		
		if quantityEncoded & self.COINSPARK_GENESIS_FLAG_CHARGE_BPS:
			self.chargeBasisPoints=self.readSmallEndianUnsigned(metadata, self.COINSPARK_GENESIS_CHARGE_BPS_LENGTH)
			metadata=metadata[self.COINSPARK_GENESIS_CHARGE_BPS_LENGTH:]
		else:
			self.chargeBasisPoints=0
	
		# Domain name and page path
		
		decodedDomainPath=self.decodeDomainAndOrPath(metadata, True, True)
		if decodedDomainPath is None:
			return False

		metadata=metadata[decodedDomainPath['decodedChars']:]			
		self.useHttps=decodedDomainPath['useHttps']
		self.domainName=decodedDomainPath['domainName']
		self.usePrefix=decodedDomainPath['usePrefix']
		self.pagePath=decodedDomainPath['pagePath']
		
		# Asset hash
	
		self.assetHashLen=min(len(metadata), COINSPARK_GENESIS_HASH_MAX_LEN)
		self.assetHash=metadata[:self.assetHashLen]
		
		# Return validity
	
		return self.isValid()

	
	def calcMinFee(self, outputsSatoshis, outputsRegular):
		if len(outputsSatoshis)!=len(outputsRegular):
			return COINSPARK_SATOSHI_QTY_MAX # these two arrays must be the same size

		return self.countNonLastRegularOutputs(outputsRegular)*self.getMinFeeBasis(outputsSatoshis, outputsRegular)

	
	def apply(self, outputsRegular):
		countOutputs=len(outputsRegular)
		lastRegularOutput=self.getLastRegularOutput(outputsRegular)
		divideOutputs=self.countNonLastRegularOutputs(outputsRegular)
		genesisQty=self.getQty()
		
		if divideOutputs==0:
			qtyPerOutput=0
		else:
			qtyPerOutput=math.floor(genesisQty/divideOutputs) # rounds down
			
		extraFirstOutput=genesisQty-qtyPerOutput*divideOutputs
		outputBalances=[0]*countOutputs
		
		for outputIndex in range(countOutputs):
			if outputsRegular[outputIndex] and (outputIndex!=lastRegularOutput):
				outputBalances[outputIndex]=qtyPerOutput+extraFirstOutput
				extraFirstOutput=0 # so it will only contribute to the first
		
		return outputBalances

	
	def calcAssetURL(self, firstSpentTxID, firstSpentVout):
		firstSpentTxIdPart=(firstSpentTxID+firstSpentTxID)[int(firstSpentVout)%64:int(firstSpentVout)%64+16]
		
		return (
			('https' if self.useHttps else 'http')+
			'://'+self.domainName+'/'+
			('coinspark/' if self.usePrefix else '')+
			(self.pagePath if len(self.pagePath) else firstSpentTxIdPart)+'/'
		).lower()

	

# CoinSparkAssetRef class for managing asset references

class CoinSparkAssetRef(CoinSparkBase):
	
	def __init__(self):
		self.clear()


	def clear(self):
		self.blockNum=0
		self.txOffset=0
		self.txIDPrefix='00' * COINSPARK_ASSETREF_TXID_PREFIX_LEN

		
	def toString(self):
		return self.toStringInner(True)
		
	
	def isValid(self):
		if self.blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE:
			if (self.blockNum<0) or (self.blockNum>COINSPARK_ASSETREF_BLOCK_NUM_MAX) or not self.isInteger(self.blockNum):
				return False
			
			if (self.txOffset<0) or (self.txOffset>COINSPARK_ASSETREF_TX_OFFSET_MAX) or not self.isInteger(self.txOffset):
				return False
				
			if (not self.isString(self.txIDPrefix)) or (len(self.txIDPrefix)!=(2*COINSPARK_ASSETREF_TXID_PREFIX_LEN)):
				return False
				
			if not all(charTest in string.hexdigits for charTest in self.txIDPrefix):
				return False
		
		return True
		
	
	def match(self, otherAssetRef):
		return ( (self.txIDPrefix.lower()==otherAssetRef.txIDPrefix.lower()) and
			(self.txOffset == otherAssetRef.txOffset) and (self.blockNum == otherAssetRef.blockNum) )
			
	
	def encode(self):
		if not self.isValid():
			return None
			
		txIDPrefixInteger=256*int(self.txIDPrefix[2:4], 16)+int(self.txIDPrefix[0:2], 16)
		
		return str(self.blockNum)+'-'+str(self.txOffset)+'-'+str(txIDPrefixInteger)
		
	
	def decode(self, _string):
		if not all(charTest in (string.digits+'-') for charTest in _string):
			return False
			
		parts=[int(x) for x in _string.split('-')]
		
		if (len(parts)!=3) or (parts[2]>0xFFFF):
			return False

		self.blockNum=parts[0]
		self.txOffset=parts[1]
		self.txIDPrefix="%02X%02X" % (parts[2]%256, math.floor(parts[2]/256))
		
		return self.isValid()
		
		
	def toStringInner(self, headers):
		buffer="COINSPARK ASSET REFERENCE\n" if headers else ""

		buffer+="Genesis block index: %.0f (small endian hex %s)\n" % (self.blockNum, self.unsignedToSmallEndianHex(self.blockNum, 4))
		buffer+=" Genesis txn offset: %.0f (small endian hex %s)\n" % (self.txOffset, self.unsignedToSmallEndianHex(self.txOffset, 4))
		buffer+="Genesis txid prefix: %s\n" % self.txIDPrefix.upper()
		
		if headers:
			buffer+="END COINSPARK ASSET REFERENCE\n\n"
			
		return buffer
		
		
	def compare(self, otherAssetRef):
		# -1 if this<otherAssetRef, 1 if otherAssetRef>this, 0 otherwise
	
		if self.blockNum!=otherAssetRef.blockNum:
			return -1 if (self.blockNum<otherAssetRef.blockNum) else 1
		elif self.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE: # in this case don't compare other fields
			return 0
		elif self.txOffset!=otherAssetRef.txOffset:
			return -1 if (self.txOffset<otherAssetRef.txOffset) else 1
		else:
			thisTxIDPrefixLower=self.txIDPrefix[:2*COINSPARK_ASSETREF_TXID_PREFIX_LEN].lower()
			otherTxIDPrefixLower=otherAssetRef.txIDPrefix[:2*COINSPARK_ASSETREF_TXID_PREFIX_LEN].lower()
		
			if thisTxIDPrefixLower!=otherTxIDPrefixLower: # comparing hex gives same order as comparing bytes
				return -1 if (thisTxIDPrefixLower<otherTxIDPrefixLower) else 1
			else:
				return 0
		

# CoinSparkTransfer class for managing individual asset transfer metadata

class CoinSparkTransfer(CoinSparkBase):
	COINSPARK_PACKING_GENESIS_MASK=0xC0
	COINSPARK_PACKING_GENESIS_PREV=0x00
	COINSPARK_PACKING_GENESIS_3_3_BYTES=0x40 # 3 bytes for block index, 3 for txn offset
	COINSPARK_PACKING_GENESIS_3_4_BYTES=0x80 # 3 bytes for block index, 4 for txn offset
	COINSPARK_PACKING_GENESIS_4_4_BYTES=0xC0 # 4 bytes for block index, 4 for txn offset

	COINSPARK_PACKING_INDICES_MASK=0x38
	COINSPARK_PACKING_INDICES_0P_0P=0x00 # input 0 only or previous, output 0 only or previous
	COINSPARK_PACKING_INDICES_0P_1S=0x08 # input 0 only or previous, output 1 only or subsequent single
	COINSPARK_PACKING_INDICES_0P_ALL=0x10 # input 0 only or previous, all outputs
	COINSPARK_PACKING_INDICES_1S_0P=0x18 # input 1 only or subsequent single, output 0 only or previous
	COINSPARK_PACKING_INDICES_ALL_0P=0x20 # all inputs, output 0 only or previous
	COINSPARK_PACKING_INDICES_ALL_1S=0x28 # all inputs, output 1 only or subsequent single
	COINSPARK_PACKING_INDICES_ALL_ALL=0x30 # all inputs, all outputs
	COINSPARK_PACKING_INDICES_EXTEND=0x38 # use second byte for more extensive information

	COINSPARK_PACKING_EXTEND_INPUTS_SHIFT=3
	COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT=0

	COINSPARK_PACKING_EXTEND_MASK=0x07
	COINSPARK_PACKING_EXTEND_0P=0x00 # index 0 only or previous
	COINSPARK_PACKING_EXTEND_1S=0x01 # index 1 only or subsequent single
	COINSPARK_PACKING_EXTEND_BYTE=0x02 # 1 byte for single index
	COINSPARK_PACKING_EXTEND_2_BYTES=0x03 # 2 bytes for single index
	COINSPARK_PACKING_EXTEND_1_1_BYTES=0x04 # 1 byte for first index, 1 byte for count
	COINSPARK_PACKING_EXTEND_2_1_BYTES=0x05 # 2 bytes for first index, 1 byte for count
	COINSPARK_PACKING_EXTEND_2_2_BYTES=0x06 # 2 bytes for first index, 2 bytes for count
	COINSPARK_PACKING_EXTEND_ALL=0x07 # all inputs|outputs

	COINSPARK_PACKING_QUANTITY_MASK=0x07
	COINSPARK_PACKING_QUANTITY_1P=0x00 # quantity=1 or previous
	COINSPARK_PACKING_QUANTITY_1_BYTE=0x01
	COINSPARK_PACKING_QUANTITY_2_BYTES=0x02
	COINSPARK_PACKING_QUANTITY_3_BYTES=0x03
	COINSPARK_PACKING_QUANTITY_4_BYTES=0x04
	COINSPARK_PACKING_QUANTITY_6_BYTES=0x05
	COINSPARK_PACKING_QUANTITY_FLOAT=0x06
	COINSPARK_PACKING_QUANTITY_MAX=0x07 # transfer all quantity across

	COINSPARK_TRANSFER_QTY_FLOAT_LENGTH=2
	COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX=1000
	COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX=11
	COINSPARK_TRANSFER_QTY_FLOAT_MASK=0x3FFF
	COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE=1001

	
	def __init__(self):
		self.clear()


	def clear(self):
		self.assetRef=CoinSparkAssetRef()
		self.inputs=CoinSparkInOutRange()
		self.outputs=CoinSparkInOutRange()
		self.qtyPerOutput=0


	def toString(self):
		return self.toStringInner(True)
		

	def isValid(self):
		if not (self.assetRef.isValid() and self.inputs.isValid() and self.outputs.isValid()):
			return False
		
		if (self.qtyPerOutput<0) or (self.qtyPerOutput>COINSPARK_ASSET_QTY_MAX) or not self.isInteger(self.qtyPerOutput):
			return False
		
		return True


	def match(self, otherTransfer):
		if self.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE:
			return ( (otherTransfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) and
				self.inputs.match(otherTransfer.inputs) and (self.outputs.first==otherTransfer.outputs.first) )
	
		else:
			return (self.assetRef.match(otherTransfer.assetRef) and
				self.inputs.match(otherTransfer.inputs) and
				self.outputs.match(otherTransfer.outputs) and
				self.qtyPerOutput==otherTransfer.qtyPerOutput)


	def encode(self, previousTransfer, metadataMaxLen, countInputs, countOutputs):
		if not self.isValid():
			return None
	
		packing=0
		packingExtend=0
		isDefaultRoute=(self.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
	
		# Packing for genesis reference
	
		if isDefaultRoute:
			if previousTransfer and (previousTransfer.assetRef.blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE):
				return None # default route transfers have to come at the start
			
			packing|=self.COINSPARK_PACKING_GENESIS_PREV
	
		else:
			if previousTransfer and self.assetRef.match(previousTransfer.assetRef):
				packing|=self.COINSPARK_PACKING_GENESIS_PREV
		
			elif self.assetRef.blockNum <= COINSPARK_UNSIGNED_3_BYTES_MAX:
				if self.assetRef.txOffset <= COINSPARK_UNSIGNED_3_BYTES_MAX:
					packing|=self.COINSPARK_PACKING_GENESIS_3_3_BYTES
				elif self.assetRef.txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX:
					packing|=self.COINSPARK_PACKING_GENESIS_3_4_BYTES
				else:
					return None

			elif (self.assetRef.blockNum <= COINSPARK_UNSIGNED_4_BYTES_MAX) and (self.assetRef.txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX):
				packing|=self.COINSPARK_PACKING_GENESIS_4_4_BYTES
		
			else:
				return None
		
		# Packing for input and output indices

		inputPackingOptions=self.getPackingOptions(previousTransfer.inputs if previousTransfer else None, self.inputs, countInputs)
		outputPackingOptions=self.getPackingOptions(previousTransfer.outputs if previousTransfer else None, self.outputs, countOutputs)
	
		if inputPackingOptions['_0P'] and outputPackingOptions['_0P']:
			packing|=self.COINSPARK_PACKING_INDICES_0P_0P
		elif inputPackingOptions['_0P'] and outputPackingOptions['_1S']:
			packing|=self.COINSPARK_PACKING_INDICES_0P_1S
		elif inputPackingOptions['_0P'] and outputPackingOptions['_ALL']:
			packing|=self.COINSPARK_PACKING_INDICES_0P_ALL
		elif inputPackingOptions['_1S'] and outputPackingOptions['_0P']:
			packing|=self.COINSPARK_PACKING_INDICES_1S_0P
		elif inputPackingOptions['_ALL'] and outputPackingOptions['_0P']:
			packing|=self.COINSPARK_PACKING_INDICES_ALL_0P
		elif inputPackingOptions['_ALL'] and outputPackingOptions['_1S']:
			packing|=self.COINSPARK_PACKING_INDICES_ALL_1S
		elif inputPackingOptions['_ALL'] and outputPackingOptions['_ALL']:
			packing|=self.COINSPARK_PACKING_INDICES_ALL_ALL

		else: # we need the second (extended) packing byte
			packing|=self.COINSPARK_PACKING_INDICES_EXTEND

			packingExtendInput=self.encodePackingExtend(inputPackingOptions)
			packingExtendOutput=self.encodePackingExtend(outputPackingOptions)

			if (packingExtendInput is None) or (packingExtendOutput is None):
				return None
			
			packingExtend=(packingExtendInput << self.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) | (packingExtendOutput << self.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT)
	
		# Packing for quantity
	
		encodeQuantity=self.qtyPerOutput
   
		if self.qtyPerOutput==(previousTransfer.qtyPerOutput if previousTransfer else 1):
			packing|=self.COINSPARK_PACKING_QUANTITY_1P
		elif self.qtyPerOutput>=COINSPARK_ASSET_QTY_MAX:
			packing|=self.COINSPARK_PACKING_QUANTITY_MAX
		elif self.qtyPerOutput<=COINSPARK_UNSIGNED_BYTE_MAX:
			packing|=self.COINSPARK_PACKING_QUANTITY_1_BYTE
		elif self.qtyPerOutput<=COINSPARK_UNSIGNED_2_BYTES_MAX:
			packing|=self.COINSPARK_PACKING_QUANTITY_2_BYTES
		else:
			result=self.qtyToMantissaExponent(self.qtyPerOutput, 0,
				self.COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX, self.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX)
			
			if result['qty']==self.qtyPerOutput:
				packing|=self.COINSPARK_PACKING_QUANTITY_FLOAT
				encodeQuantity=(result['exponent']*self.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+result['mantissa'])&self.COINSPARK_TRANSFER_QTY_FLOAT_MASK
		
			elif self.qtyPerOutput<=COINSPARK_UNSIGNED_3_BYTES_MAX:
				packing|=self.COINSPARK_PACKING_QUANTITY_3_BYTES
			elif self.qtyPerOutput<=COINSPARK_UNSIGNED_4_BYTES_MAX:
				packing|=self.COINSPARK_PACKING_QUANTITY_4_BYTES
			else:
				packing|=self.COINSPARK_PACKING_QUANTITY_6_BYTES
		
		# Write out the actual data

		counts=self.packingToByteCounts(packing, packingExtend)
	
		metadata=chr(packing)
	
		if (packing & self.COINSPARK_PACKING_INDICES_MASK) == self.COINSPARK_PACKING_INDICES_EXTEND:
			metadata=metadata+chr(packingExtend)
	
		written_array=[
			self.writeUnsignedField(counts['blockNumBytes'], self.assetRef.blockNum),
			self.writeUnsignedField(counts['txOffsetBytes'], self.assetRef.txOffset),
			(CoinSparkHexToRawString(self.assetRef.txIDPrefix)+("\x00" * counts['txIDPrefixBytes']))[:counts['txIDPrefixBytes']], # ensure right length
			self.writeUnsignedField(counts['firstInputBytes'], self.inputs.first),
			self.writeUnsignedField(counts['countInputsBytes'], self.inputs.count),
			self.writeUnsignedField(counts['firstOutputBytes'], self.outputs.first),
			self.writeUnsignedField(counts['countOutputsBytes'], self.outputs.count),
			self.writeUnsignedField(counts['quantityBytes'], encodeQuantity)
		]
		
		for written in written_array:
			if written is None:
				return None
			else:
				metadata+=written
			
		# Check the total length is within the specified limit

		if len(metadata)>metadataMaxLen:
			return None
		
		# Return what we created

		return metadata			


	def decode(self, metadata, previousTransfer, countInputs, countOutputs):
		startLength=len(metadata)
	
		# Extract packing

		packing=self.readSmallEndianUnsigned(metadata, 1)
		metadata=metadata[1:]
		if packing is None:
			return 0

		packingExtend=0
		
		# Packing for genesis reference

		if (packing & self.COINSPARK_PACKING_GENESIS_MASK)==self.COINSPARK_PACKING_GENESIS_PREV:
			if previousTransfer:
				self.assetRef=previousTransfer.assetRef
			
			else: # it's for a default route
				self.assetRef.blockNum=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE
				self.assetRef.txOffset=0
				self.assetRef.txIDPrefix="00" * COINSPARK_ASSETREF_TXID_PREFIX_LEN


		# Packing for input and output indices

		if (packing & self.COINSPARK_PACKING_INDICES_MASK) == self.COINSPARK_PACKING_INDICES_EXTEND: # we're using second packing metadata byte
			packingExtend=self.readSmallEndianUnsigned(metadata, 1)
			metadata=metadata[1:]
			if packingExtend is None:
				return 0
			
			inputPackingType=self.decodePackingExtend((packingExtend >> self.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & self.COINSPARK_PACKING_EXTEND_MASK)
			outputPackingType=self.decodePackingExtend((packingExtend >> self.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & self.COINSPARK_PACKING_EXTEND_MASK)

			if (inputPackingType is None) or (outputPackingType is None):
				return 0

		else: # not using second packing metadata byte

			packingIndices=packing & self.COINSPARK_PACKING_INDICES_MASK
			
			# input packing
			
			if (
				(packingIndices==self.COINSPARK_PACKING_INDICES_0P_0P) or
				(packingIndices==self.COINSPARK_PACKING_INDICES_0P_1S) or
				(packingIndices==self.COINSPARK_PACKING_INDICES_0P_ALL)
			):
				inputPackingType='_0P'
				
			elif packingIndices==self.COINSPARK_PACKING_INDICES_1S_0P:
				inputPackingType='_1S'
				
			elif (
				(packingIndices==self.COINSPARK_PACKING_INDICES_ALL_0P) or
				(packingIndices==self.COINSPARK_PACKING_INDICES_ALL_1S) or
				(packingIndices==self.COINSPARK_PACKING_INDICES_ALL_ALL)
			):
				inputPackingType='_ALL'
			
			# output packing
			
			if (
				(packingIndices==self.COINSPARK_PACKING_INDICES_0P_0P) or
				(packingIndices==self.COINSPARK_PACKING_INDICES_1S_0P) or
				(packingIndices==self.COINSPARK_PACKING_INDICES_ALL_0P)
			):
				outputPackingType='_0P'
				
			elif (
				(packingIndices==self.COINSPARK_PACKING_INDICES_0P_1S) or
				(packingIndices==self.COINSPARK_PACKING_INDICES_ALL_1S)
			):
				outputPackingType='_1S'
				
			elif (
				(packingIndices==self.COINSPARK_PACKING_INDICES_0P_ALL) or
				(packingIndices==self.COINSPARK_PACKING_INDICES_ALL_ALL)
			):
				outputPackingType='_ALL'
			

		# Final stage of packing for input and output indices

		self.inputs=self.packingTypeToValues(inputPackingType, previousTransfer.inputs if previousTransfer else None, countInputs)
		self.outputs=self.packingTypeToValues(outputPackingType, previousTransfer.outputs if previousTransfer else None, countOutputs)

		# Read in the fields as appropriate

		counts=self.packingToByteCounts(packing, packingExtend)
		
		txIDPrefixBytes=counts['txIDPrefixBytes']
		
		metadataArray=[metadataChar for metadataChar in metadata] # split into array of characters for next bit
		
		read_array=[
			self.readUnsignedField(metadataArray, counts['blockNumBytes'], self.assetRef, 'blockNum'),
			self.readUnsignedField(metadataArray, counts['txOffsetBytes'], self.assetRef, 'txOffset')
		]
		
		if txIDPrefixBytes==0:
			read_array.append(True)
		else:
			self.assetRef.txIDPrefix=CoinSparkRawStringToHex("".join(metadataArray[:txIDPrefixBytes]))
			metadataArray=metadataArray[txIDPrefixBytes:]
			read_array.append(len(self.assetRef.txIDPrefix)==2*txIDPrefixBytes)
				
		read_array+=[
			self.readUnsignedField(metadataArray, counts['firstInputBytes'], self.inputs, 'first'),
			self.readUnsignedField(metadataArray, counts['countInputsBytes'], self.inputs, 'count'),
			self.readUnsignedField(metadataArray, counts['firstOutputBytes'], self.outputs, 'first'),
			self.readUnsignedField(metadataArray, counts['countOutputsBytes'], self.outputs, 'count'),
			self.readUnsignedField(metadataArray, counts['quantityBytes'], self, 'qtyPerOutput')
		]
		
		metadata="".join(metadataArray) # convert any remaining characters back into the string
		
		for read in read_array:
			if not read:
				return 0

		# Finish up reading in quantity
		
		packingQuantity=packing & self.COINSPARK_PACKING_QUANTITY_MASK
		
		if packingQuantity==self.COINSPARK_PACKING_QUANTITY_1P:
			if previousTransfer:
				self.qtyPerOutput=previousTransfer.qtyPerOutput
			else:
				self.qtyPerOutput=1
		
		elif packingQuantity==self.COINSPARK_PACKING_QUANTITY_MAX:
			self.qtyPerOutput=COINSPARK_ASSET_QTY_MAX
			
		elif packingQuantity==self.COINSPARK_PACKING_QUANTITY_FLOAT:
			decodeQuantity=self.qtyPerOutput&self.COINSPARK_TRANSFER_QTY_FLOAT_MASK
			self.qtyPerOutput=self.mantissaExponentToQty(decodeQuantity%self.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE,
				math.floor(decodeQuantity/self.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE))

		# Return bytes used
		
		if not self.isValid():
			return 0
		
		return startLength-len(metadata)


	def toStringInner(self, headers):
		buffer="COINSPARK TRANSFER\n" if headers else ""
		isDefaultRoute=(self.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
	
		if isDefaultRoute:
			buffer+="      Default route:\n"
	
		else:
			buffer+=self.assetRef.toStringInner(False)
			buffer+="    Asset reference: "+self.assetRef.encode()+"\n"

		if self.inputs.count>0:
			if self.inputs.count>1:
				buffer+="             Inputs: %d - %d (count %d)" % (self.inputs.first, self.inputs.first+self.inputs.count-1, self.inputs.count)
			else:
				buffer+="              Input: %d" % self.inputs.first
		else:
			buffer+="             Inputs: none"
		
		buffer+=(" (small endian hex: first "+self.unsignedToSmallEndianHex(self.inputs.first, 2)+" count "+
			self.unsignedToSmallEndianHex(self.inputs.count, 2)+")\n")

		if self.outputs.count>0:
			if (self.outputs.count>1) and not isDefaultRoute:
				buffer+="            Outputs: %d - %d (count %d)" % (self.outputs.first, self.outputs.first+self.outputs.count-1, self.outputs.count)
			else:
				buffer+="             Output: %d" % self.outputs.first
		else:
			buffer+="            Outputs: none"
	
		buffer+=(" (small endian hex: first "+self.unsignedToSmallEndianHex(self.outputs.first, 2)+
			" count "+self.unsignedToSmallEndianHex(self.outputs.count, 2)+")\n")
	
		if not isDefaultRoute:
			buffer+="     Qty per output: "+str(self.qtyPerOutput)+" (small endian hex "+self.unsignedToSmallEndianHex(self.qtyPerOutput, 8)
		
			result=self.qtyToMantissaExponent(self.qtyPerOutput, 0,
				self.COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX, self.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX)
		
			if result['qty']==self.qtyPerOutput:
				encodeQuantity=(result['exponent']*self.COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+result['mantissa'])&self.COINSPARK_TRANSFER_QTY_FLOAT_MASK		
				buffer+=", as float "+self.unsignedToSmallEndianHex(encodeQuantity, self.COINSPARK_TRANSFER_QTY_FLOAT_LENGTH)

			buffer+=")\n"
	
		if headers:
			buffer+="END COINSPARK TRANSFER\n\n"
	
		return buffer


	def getPackingOptions(self, previousRange, range, countInputsOutputs):
		packingOptions={}
	
		firstZero=(range.first==0)
		firstByte=(range.first<=COINSPARK_UNSIGNED_BYTE_MAX)
		first2Bytes=(range.first<=COINSPARK_UNSIGNED_2_BYTES_MAX)
		countOne=(range.count==1)
		countByte=(range.count<=COINSPARK_UNSIGNED_BYTE_MAX)
	
		if previousRange:
			packingOptions['_0P']=(range.first==previousRange.first) and (range.count==previousRange.count)
			packingOptions['_1S']=(range.first==(previousRange.first+previousRange.count)) and countOne
		
		else:
			packingOptions['_0P']=firstZero and countOne
			packingOptions['_1S']=(range.first==1) and countOne

		packingOptions['_BYTE']=firstByte and countOne
		packingOptions['_2_BYTES']=first2Bytes and countOne
		packingOptions['_1_1_BYTES']=firstByte and countByte
		packingOptions['_2_1_BYTES']=first2Bytes and countByte
		packingOptions['_2_2_BYTES']=first2Bytes and (range.count<=COINSPARK_UNSIGNED_2_BYTES_MAX)
		packingOptions['_ALL']=firstZero and (range.count>=int(countInputsOutputs))
	
		return packingOptions


	def packingTypeToValues(self, packingType, previousRange, countInputOutputs):
		range=CoinSparkInOutRange()
		
		if packingType=='_0P':
			if previousRange:
				range.first=previousRange.first
				range.count=previousRange.count
			else:
				range.first=0
				range.count=1

		elif packingType=='_1S':
			if previousRange:
				range.first=previousRange.first+previousRange.count
			else:
				range.first=1

			range.count=1

		elif (packingType=='_BYTE') or (packingType=='_2_BYTES'):
			range.count=1

		elif packingType=='_ALL':
			range.first=0
			range.count=int(countInputOutputs)
	
		return range


	def packingToByteCounts(self, packing, packingExtend):

		# Set default values for bytes for all fields to zero

		counts={
			'blockNumBytes': 0,
			'txOffsetBytes': 0,
			'txIDPrefixBytes': 0,
		
			'firstInputBytes': 0,
			'countInputsBytes': 0,
			'firstOutputBytes': 0,
			'countOutputsBytes': 0,
		
			'quantityBytes': 0
		}
	
		# Packing for genesis reference
		
		packingGenesis=packing & self.COINSPARK_PACKING_GENESIS_MASK

		if packingGenesis==self.COINSPARK_PACKING_GENESIS_3_3_BYTES:
			counts['blockNumBytes']=3
			counts['txOffsetBytes']=3
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN

		elif packingGenesis==self.COINSPARK_PACKING_GENESIS_3_4_BYTES:
			counts['blockNumBytes']=3
			counts['txOffsetBytes']=4
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN

		elif packingGenesis==self.COINSPARK_PACKING_GENESIS_4_4_BYTES:
			counts['blockNumBytes']=4
			counts['txOffsetBytes']=4
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN

		# Packing for input and output indices (relevant for extended indices only)

		if (packing & self.COINSPARK_PACKING_INDICES_MASK) == self.COINSPARK_PACKING_INDICES_EXTEND:

			# Input indices
			
			packingExtendInputs=(packingExtend >> self.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & self.COINSPARK_PACKING_EXTEND_MASK

			if packingExtendInputs==self.COINSPARK_PACKING_EXTEND_BYTE:
				counts['firstInputBytes']=1
	
			elif packingExtendInputs==self.COINSPARK_PACKING_EXTEND_2_BYTES:
				counts['firstInputBytes']=2
	
			elif packingExtendInputs==self.COINSPARK_PACKING_EXTEND_1_1_BYTES:
				counts['firstInputBytes']=1
				counts['countInputsBytes']=1
	
			elif packingExtendInputs==self.COINSPARK_PACKING_EXTEND_2_1_BYTES:
				counts['firstInputBytes']=2
				counts['countInputsBytes']=1
	
			elif packingExtendInputs==self.COINSPARK_PACKING_EXTEND_2_2_BYTES:
				counts['firstInputBytes']=2
				counts['countInputsBytes']=2

			# Output indices
			
			packingExtendOutputs=(packingExtend >> self.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & self.COINSPARK_PACKING_EXTEND_MASK

			if packingExtendOutputs==self.COINSPARK_PACKING_EXTEND_BYTE:
				counts['firstOutputBytes']=1
	
			elif packingExtendOutputs==self.COINSPARK_PACKING_EXTEND_2_BYTES:
				counts['firstOutputBytes']=2
	
			elif packingExtendOutputs==self.COINSPARK_PACKING_EXTEND_1_1_BYTES:
				counts['firstOutputBytes']=1
				counts['countOutputsBytes']=1
	
			elif packingExtendOutputs==self.COINSPARK_PACKING_EXTEND_2_1_BYTES:
				counts['firstOutputBytes']=2
				counts['countOutputsBytes']=1
	
			elif packingExtendOutputs==self.COINSPARK_PACKING_EXTEND_2_2_BYTES:
				counts['firstOutputBytes']=2
				counts['countOutputsBytes']=2

		# Packing for quantity
		
		packingQuantity=packing & self.COINSPARK_PACKING_QUANTITY_MASK

		if packingQuantity==self.COINSPARK_PACKING_QUANTITY_1_BYTE:
			counts['quantityBytes']=1

		elif packingQuantity==self.COINSPARK_PACKING_QUANTITY_2_BYTES:
			counts['quantityBytes']=2

		elif packingQuantity==self.COINSPARK_PACKING_QUANTITY_3_BYTES:
			counts['quantityBytes']=3

		elif packingQuantity==self.COINSPARK_PACKING_QUANTITY_4_BYTES:
			counts['quantityBytes']=4

		elif packingQuantity==self.COINSPARK_PACKING_QUANTITY_6_BYTES:
			counts['quantityBytes']=6

		elif packingQuantity==self.COINSPARK_PACKING_QUANTITY_FLOAT:
			counts['quantityBytes']=self.COINSPARK_TRANSFER_QTY_FLOAT_LENGTH

		# Return the resulting array
	
		return counts


	def getPackingExtendMap(self):
		 return [
			('_0P', self.COINSPARK_PACKING_EXTEND_0P),
			('_1S', self.COINSPARK_PACKING_EXTEND_1S),
			('_ALL', self.COINSPARK_PACKING_EXTEND_ALL),
			('_BYTE', self.COINSPARK_PACKING_EXTEND_BYTE),
			('_2_BYTES', self.COINSPARK_PACKING_EXTEND_2_BYTES),
			('_1_1_BYTES', self.COINSPARK_PACKING_EXTEND_1_1_BYTES),
			('_2_1_BYTES', self.COINSPARK_PACKING_EXTEND_2_1_BYTES),
			('_2_2_BYTES', self.COINSPARK_PACKING_EXTEND_2_2_BYTES)
		] # in order of preference (use tuples since Python dictionaries are unordered)


	def encodePackingExtend(self, packingOptions):
		packingExtendMap=self.getPackingExtendMap()
	
		for (packingType, packingExtend) in packingExtendMap:
			if packingOptions[packingType]:
				return packingExtend
			
		return None


	def decodePackingExtend(self, packingExtend):
		packingExtendMap=self.getPackingExtendMap()
	
		for (packingType, packingExtendTest) in packingExtendMap:
			if packingExtend==packingExtendTest:
				return packingType
			
		return None


	def writeUnsignedField(self, bytes, source):
		return self.writeSmallEndianUnsigned(source, bytes) if (bytes>0) else '' # will return None on failure


	def readUnsignedField(self, metadataArray, bytes, object, property):
		if bytes>0:
			value=self.readSmallEndianUnsigned(metadataArray[:bytes], bytes)
			del metadataArray[:bytes]
			if value is None:
				return False
			
			setattr(object, property, value)
	
		return True


# CoinSparkTransferList class for managing list of asset transfer metadata

class CoinSparkTransferList(CoinSparkBase):

	def __init__(self):
		self.clear()

	
	def clear(self):
		self.transfers=[]


	def toString(self):
		buffer="COINSPARK TRANSFERS\n"

		for transferIndex in range(len(self.transfers)):
			if transferIndex>0:
				buffer+="\n"
		
			buffer+=self.transfers[transferIndex].toStringInner(False)

		buffer+="END COINSPARK TRANSFERS\n\n"

		return buffer
		
		
	def isValid(self):
		if not isinstance(self.transfers, list):
			return False
			
		for transfer in self.transfers:
			if not transfer.isValid():
				return False
				
		return True


	def match(self, otherTransfers, strict):
		countTransfers=len(self.transfers)
		if countTransfers!=len(otherTransfers.transfers):
			return False
	
		if strict:
			for transferIndex in range(countTransfers):
				if not self.transfers[transferIndex].match(otherTransfers.transfers[transferIndex]):
					return False
	
		else:
			thisOrdering=self.groupOrdering()
			otherOrdering=otherTransfers.groupOrdering()
		
			for transferIndex in range(countTransfers):
				if not self.transfers[thisOrdering[transferIndex]].match(otherTransfers.transfers[otherOrdering[transferIndex]]):
					return False

		return True


	def encode(self, countInputs, countOutputs, metadataMaxLen):

		# 4-character identifier

		metadata=COINSPARK_METADATA_IDENTIFIER+COINSPARK_TRANSFERS_PREFIX

		# Encode each transfer, grouping by asset reference, but preserving original order otherwise

		ordering=self.groupOrdering()
	
		countTransfers=len(self.transfers)
		previousTransfer=None
	
		for transferIndex in range(countTransfers):
			thisTransfer=self.transfers[ordering[transferIndex]]
		
			written=thisTransfer.encode(previousTransfer, metadataMaxLen-len(metadata), countInputs, countOutputs)
			if written is None:
				return None
			
			metadata+=written
			previousTransfer=thisTransfer

		# Extra length check (even though thisTransfer.encode() should be sufficient)

		if len(metadata)>metadataMaxLen:
			return None
		
		# Return what we created

		return metadata


	def decode(self, metadata, countInputs, countOutputs):
		metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_TRANSFERS_PREFIX)
		if metadata is None:
			return 0

		# Iterate over list

		self.transfers=[]
		previousTransfer=None

		while len(metadata)>0:
			transfer=CoinSparkTransfer()
			transferBytesUsed=transfer.decode(metadata, previousTransfer, countInputs, countOutputs)
		
			if transferBytesUsed>0:
				self.transfers.append(transfer)
				metadata=metadata[transferBytesUsed:]
				previousTransfer=transfer
		
			else:
				return 0 # something was invalid


		# Return count

		return len(self.transfers)


	def calcMinFee(self, countInputs, outputsSatoshis, outputsRegular):
		countOutputs=len(outputsSatoshis)
		if countOutputs!=len(outputsRegular):
			return COINSPARK_SATOSHI_QTY_MAX # these two arrays must be the same size
	
		transfersToCover=0
	
		for transfer in self.transfers:
			if (
				(transfer.assetRef.blockNum != COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) and # don't count default routes
				(transfer.inputs.count>0) and
				(transfer.inputs.first<countInputs) # only count if at least one valid input index
			):
				outputIndex=max(transfer.outputs.first, 0)
				lastOutputIndex=min(transfer.outputs.first+transfer.outputs.count, countOutputs)-1
				
				while outputIndex<=lastOutputIndex:
					if outputsRegular[outputIndex]:
						transfersToCover+=1
					outputIndex+=1
	
		return transfersToCover*self.getMinFeeBasis(outputsSatoshis, outputsRegular)


	def apply(self, assetRef, genesis, _inputBalances, outputsRegular):
		inputBalances=[float(inputBalance) for inputBalance in _inputBalances] # copy since we will modify it, and cast to integers

		# Zero output quantities and get counts
	
		countInputs=len(inputBalances)
		countOutputs=len(outputsRegular)
		outputBalances=[0] * countOutputs
	
		# Perform explicit transfers (i.e. not default routes)
		
		for transfer in self.transfers:
			if assetRef.match(transfer.assetRef):
				inputIndex=max(transfer.inputs.first, 0)
				outputIndex=max(transfer.outputs.first, 0)
	
				lastInputIndex=min(inputIndex+transfer.inputs.count, countInputs)-1
				lastOutputIndex=min(outputIndex+transfer.outputs.count, countOutputs)-1
	
				while outputIndex<=lastOutputIndex:
					if outputsRegular[outputIndex]:
						transferRemaining=transfer.qtyPerOutput
			
						while inputIndex<=lastInputIndex:
							transferQuantity=min(transferRemaining, inputBalances[inputIndex])
				
							if transferQuantity>0: # skip all this if nothing is to be transferred (branch not really necessary)
								inputBalances[inputIndex]-=transferQuantity
								transferRemaining-=transferQuantity
								outputBalances[outputIndex]+=transferQuantity
				
							if transferRemaining>0:
								inputIndex+=1 # move to next input since self one is drained
							else:
								break # stop if we have nothing left to transfer

					outputIndex+=1
	
		# Apply payment charges to all quantities not routed by default

		for outputIndex in range(countOutputs):
			if outputsRegular[outputIndex]:
				outputBalances[outputIndex]=genesis.calcNet(outputBalances[outputIndex])
			
		# Send remaining quantities to default outputs

		inputDefaultOutput=self.getDefaultRouteMap(countInputs, outputsRegular)
		
		for inputIndex in range(len(inputDefaultOutput)):
			outputIndex=inputDefaultOutput[inputIndex]
	
			if not outputIndex is None:
				outputBalances[outputIndex]+=inputBalances[inputIndex]
			
		# Return the result

		return outputBalances


	def applyNone(self, assetRef, genesis, inputBalances, outputsRegular):
		countOutputs=len(outputsRegular)
		outputBalances=[0] * countOutputs

		outputIndex=self.getLastRegularOutput(outputsRegular)
		if not outputIndex is None:
			for inputBalance in inputBalances:
				outputBalances[outputIndex]+=float(inputBalance) # to prevent concatenation
		
		return outputBalances

	
	def defaultOutputs(self, countInputs, outputsRegular):
		outputsDefault=[False] * len(outputsRegular)
	
		inputDefaultOutput=self.getDefaultRouteMap(countInputs, outputsRegular)
		
		for outputIndex in inputDefaultOutput:
			if not outputIndex is None:
				outputsDefault[outputIndex]=True
			
		return outputsDefault


	def groupOrdering(self):
		countTransfers=len(self.transfers)
		transferUsed=[False] * countTransfers
		ordering=[None] * countTransfers

		for orderIndex in range(countTransfers):
			bestTransferScore=0
			bestTransferIndex=-1
		
			for transferIndex in range(countTransfers):
				transfer=self.transfers[transferIndex]
			
				if not transferUsed[transferIndex]:
					if transfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE:
						transferScore=3 # top priority to default routes, which must be first in the encoded list
					elif (orderIndex>0) and transfer.assetRef.match(self.transfers[ordering[orderIndex-1]].assetRef):
						transferScore=2 # then next best is one which has same asset reference as previous
					else:
						transferScore=1 # otherwise any will do
					
					if transferScore>bestTransferScore: # if it's clearly the best, take it
						bestTransferScore=transferScore
						bestTransferIndex=transferIndex
					
					elif transferScore==bestTransferScore: # otherwise give priority to "lower" asset references
						if transfer.assetRef.compare(self.transfers[bestTransferIndex].assetRef)<0:
							bestTransferIndex=transferIndex
			
			ordering[orderIndex]=bestTransferIndex
			transferUsed[bestTransferIndex]=True
	
		return ordering


	def getDefaultRouteMap(self, countInputs, outputsRegular):
		countOutputs=len(outputsRegular)
	
		# Default to last output for all inputs

		inputDefaultOutput=[self.getLastRegularOutput(outputsRegular)] * countInputs
	
		# Apply any default route transfers in reverse order (since early ones take precedence)
		
		for transfer in reversed(self.transfers):
			if transfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE:
				outputIndex=transfer.outputs.first
			
				if (outputIndex>=0) and (outputIndex<countOutputs):
					inputIndex=max(transfer.inputs.first, 0)
					lastInputIndex=min(inputIndex+transfer.inputs.count, countInputs)-1
					
					while inputIndex<=lastInputIndex:
						inputDefaultOutput[inputIndex]=outputIndex
						inputIndex+=1
		
		# Return the result
		
		return inputDefaultOutput


# CoinSparkPaymentRef class for managing payment references

class CoinSparkPaymentRef(CoinSparkBase):

	def __init__(self):
		self.clear()


	def clear(self):
		self.ref=0
		
	
	def toString(self):
		buffer="COINSPARK PAYMENT REFERENCE\n"
		buffer+=str(self.ref)+" (small endian hex "+self.unsignedToSmallEndianHex(self.ref, 8)+")\n"
		buffer+="END COINSPARK PAYMENT REFERENCE\n\n"
	
		return buffer


	def isValid(self):
		return self.isInteger(self.ref) and (self.ref>=0) and (self.ref<=COINSPARK_PAYMENT_REF_MAX)
		
		
	def match(self, otherPaymentRef):
		return self.ref==otherPaymentRef.ref
		
		
	def randomize(self):
		self.ref=0
		
		bitsRemaining=COINSPARK_PAYMENT_REF_MAX
		while bitsRemaining>0:
			self.ref*=8192
			self.ref+=random.randint(0, 8191)
			bitsRemaining=math.floor(bitsRemaining/8192)
	
		self.ref=round(self.ref%(1+COINSPARK_PAYMENT_REF_MAX))	
		
		return self.ref


	def encode(self, metadataMaxLen):
		if not self.isValid():
			return None
		
		# 4-character identifier
		
		metadata=COINSPARK_METADATA_IDENTIFIER+COINSPARK_PAYMENTREF_PREFIX

		# The payment reference
	
		bytes=0
		paymentLeft=self.ref
		while (paymentLeft>0):
			bytes+=1
			paymentLeft=math.floor(paymentLeft/256)
		
		metadata=metadata+self.writeSmallEndianUnsigned(self.ref, bytes)
	
		# Check the total length is within the specified limit

		if len(metadata)>metadataMaxLen:
			return None
		
		# Return what we created

		return metadata


	def decode(self, metadata):
		metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_PAYMENTREF_PREFIX)
		if metadata is None:
			return None
		
		# The payment reference

		finalMetadataLen=len(metadata)
		if finalMetadataLen>8:
			return None

		self.ref=self.readSmallEndianUnsigned(metadata, finalMetadataLen)
		
		# Return validity

		return self.isValid()	


# Class used internally for input or output ranges

class CoinSparkInOutRange(CoinSparkBase):

	def __init__(self):
		self.clear()


	def clear(self):
		self.first=0
		self.count=0


	def isValid(self):
		if (self.first<0) or (self.first>COINSPARK_IO_INDEX_MAX) or not self.isInteger(self.first):
			return False
		
		if (self.count<0) or (self.count>COINSPARK_IO_INDEX_MAX) or not self.isInteger(self.count):
			return False
	
		return True


	def match(self, otherInOutRange):
		return (self.first==otherInOutRange.first) and (self.count==otherInOutRange.count)


# Other function used internally

def CoinSparkGetRawScript(scriptPubKey, scriptIsHex):
	if scriptIsHex:
		return CoinSparkHexToRawString(scriptPubKey)
	else:
		return scriptPubKey


def CoinSparkLocateMetadataRange(metadata, desiredPrefix):
	metadataLen=len(metadata)

	if metadataLen<(COINSPARK_METADATA_IDENTIFIER_LEN+1): # check for 4 bytes at least
		return None 
	
	if metadata[:COINSPARK_METADATA_IDENTIFIER_LEN]!=COINSPARK_METADATA_IDENTIFIER: # check it starts 'SPK'
		return None 
	
	position=COINSPARK_METADATA_IDENTIFIER_LEN # start after 'SPK'

	while position<metadataLen:
		foundPrefix=metadata[position] # read the next prefix
		position+=1
		foundPrefixOrd=ord(foundPrefix)
		
		if (foundPrefixOrd>COINSPARK_LENGTH_PREFIX_MAX) if (desiredPrefix is None) else (foundPrefix==desiredPrefix):
			# it's our data from here to the end (if desiredPrefix is None, it matches the last one whichever it is)
			return metadata[position:]
	
		if foundPrefixOrd>COINSPARK_LENGTH_PREFIX_MAX: # it's some other type of data from here to end
			return None
		
		# if we get here it means we found a length byte
	
		if (position+foundPrefixOrd)>metadataLen: # something went wrong - length indicated is longer than that available
			return None
		
		if position>=metadataLen: # something went wrong - that was the end of the input data
			return None 
		
		if metadata[position]==desiredPrefix: # it's the length of our part
			return metadata[position+1:position+foundPrefixOrd]
		else:
			position+=foundPrefixOrd # skip over this many bytes

	return None
	

def CoinSparkHexToRawString(hex):
	if (len(hex)%2) or not all(charTest in string.hexdigits for charTest in hex):
		return None
	else:
		raw=binascii.a2b_hex(hex)
		if not isinstance(raw, str): # to support Python 3
			raw="".join(map(chr, raw))
	
	return raw
	

def CoinSparkRawStringToHex(string):
	return "".join(["%0.2X" % ord(s) for s in string])
	