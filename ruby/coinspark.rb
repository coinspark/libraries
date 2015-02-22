# CoinSpark 2.1 - Ruby library
#
# Copyright (c) Coin Sciences Ltd
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


# Constants for use by clients of the library

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

COINSPARK_MESSAGE_SERVER_HOST_MAX_LEN = 32
COINSPARK_MESSAGE_SERVER_PATH_MAX_LEN = 24
COINSPARK_MESSAGE_HASH_MIN_LEN = 12
COINSPARK_MESSAGE_HASH_MAX_LEN = 32
COINSPARK_MESSAGE_MAX_IO_RANGES = 16

COINSPARK_IO_INDEX_MAX = 65535
	
COINSPARK_ADDRESS_FLAG_ASSETS = 1
COINSPARK_ADDRESS_FLAG_PAYMENT_REFS = 2
COINSPARK_ADDRESS_FLAG_TEXT_MESSAGES = 4
COINSPARK_ADDRESS_FLAG_FILE_MESSAGES = 8
COINSPARK_ADDRESS_FLAG_MASK = 0x7FFFFF # 23 bits are currently usable


# Constants for internal use only
		
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
COINSPARK_MESSAGE_PREFIX = 'm'

COINSPARK_FEE_BASIS_MAX_SATOSHIS = 1000

COINSPARK_INTEGER_TO_BASE_58="123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
	
COINSPARK_DOMAIN_PACKING_PREFIX_MASK = 0xC0
COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT = 6
COINSPARK_DOMAIN_PACKING_SUFFIX_MASK = 0x3F
COINSPARK_DOMAIN_PACKING_SUFFIX_MAX = 61
COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4_NO_PATH = 62
COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4 = 63
COINSPARK_DOMAIN_PACKING_IPv4_HTTPS = 0x40
COINSPARK_DOMAIN_PACKING_IPv4_NO_PATH_PREFIX = 0x80

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

def CoinSparkScriptToMetadata(scriptPubKey, scriptIsHex)
	scriptPubKeyRaw=CoinSparkGetRawScript(scriptPubKey, scriptIsHex)
	scriptPubKeyRawLen=scriptPubKeyRaw.length
	metadataLen=scriptPubKeyRawLen-2
	
	return scriptPubKeyRaw[2..-1] if (
		(scriptPubKeyRawLen>2) &&
		(scriptPubKeyRaw[0].ord == 0x6a) &&
		(scriptPubKeyRaw[1].ord > 0) &&
		(scriptPubKeyRaw[1].ord <= 75) &&
		(scriptPubKeyRaw[1].ord  == metadataLen)
	)
		
	return nil
end
	

def CoinSparkScriptsToMetadata(scriptPubKeys, scriptsAreHex)
	for scriptPubKey in scriptPubKeys
		if !CoinSparkScriptIsRegular(scriptPubKey, scriptsAreHex)
			return CoinSparkScriptToMetadata(scriptPubKey, scriptsAreHex)
		end
	end
			
	return nil
end
	

def CoinSparkMetadataToScript(metadata, toHexScript)
	if metadata.length<=75
		scriptPubKey=0x6a.chr+metadata.length.chr+metadata
		if toHexScript
			scriptPubKey=scriptPubKey.unpack('H*')[0].upcase
		end
			
		return scriptPubKey
	end
		
	return nil
end


def CoinSparkMetadataMaxAppendLen(metadata, metadataMaxLen)
	return [metadataMaxLen-(metadata.length+1-COINSPARK_METADATA_IDENTIFIER_LEN), 0].max
end


def CoinSparkMetadataAppend(metadata, metadataMaxLen, appendMetadata)
	lastMetadata=CoinSparkLocateMetadataRange(metadata, nil) # check we can find last metadata

	return nil if lastMetadata==nil

	return nil if appendMetadata.length<(COINSPARK_METADATA_IDENTIFIER_LEN+1) # check there is enough to check the prefix
		
	return nil if appendMetadata[0...COINSPARK_METADATA_IDENTIFIER_LEN]!=COINSPARK_METADATA_IDENTIFIER # then check the prefix
	
	# we don't check the character after the prefix in appendMetadata because it could itself be composite
	
	needLength=metadata.length+appendMetadata.length-COINSPARK_METADATA_IDENTIFIER_LEN+1 # check there is enough space
	return nil if metadataMaxLen<needLength
	
	lastMetadataLen=lastMetadata.length+1 # include prefix
	lastMetadataPos=metadata.length-lastMetadataLen
	
	return metadata[0...lastMetadataPos]+lastMetadataLen.chr+metadata[lastMetadataPos..-1]+appendMetadata[COINSPARK_METADATA_IDENTIFIER_LEN..-1]
end


def CoinSparkScriptIsRegular(scriptPubKey, scriptIsHex)
	scriptPubKeyRaw=CoinSparkGetRawScript(scriptPubKey, scriptIsHex)
			
	return (scriptPubKeyRaw.length<1) || (scriptPubKeyRaw[0].ord!=0x6a)
end


def CoinSparkCalcAssetHash(name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent)
	require 'digest'
	
	regexp=/(^[\x09\x0A\x0D\x20]+)|([\x09\x0A\x0D\x20]+$)/
	
	buffer=name.to_s.gsub(regexp, '')+"\x00"
	buffer+=issuer.to_s.gsub(regexp, '')+"\x00"
	buffer+=description.to_s.gsub(regexp, '')+"\x00"
	buffer+=units.to_s.gsub(regexp, '')+"\x00"
	buffer+=issueDate.to_s.gsub(regexp, '')+"\x00"
	buffer+=expiryDate.to_s.gsub(regexp, '')+"\x00"
	
	interestRateToHash=(interestRate.to_f*1000000.0+0.5).floor
	multipleToHash=(((multiple==nil) ? 1 : multiple.to_f)*1000000.0+0.5).floor
	
	buffer+=sprintf("%.0f", interestRateToHash)+"\x00"
	buffer+=sprintf("%.0f", multipleToHash)+"\x00"
	
	buffer.force_encoding('BINARY')
	
	buffer+=contractContent if contractContent!=nil
		
	buffer+="\x00"
	
	return Digest::SHA256.digest buffer
end

def CoinSparkCalcMessageHash(salt, messageParts)
	require 'digest'
	
	buffer=salt+"\x00"
	
	for messagePart in messageParts
		buffer+=messagePart['mimeType'].force_encoding('BINARY')+"\x00"
		buffer+=(messagePart.key?('fileName') ? messagePart['fileName'].force_encoding('BINARY') : "")+"\x00"
		buffer+=messagePart['content']+"\x00"
	end
		
	return Digest::SHA256.digest buffer
end


# Base class implementing utility functions used internally

class CoinSparkBase

	COINSPARK_PACKING_EXTEND_MASK=0x07
	COINSPARK_PACKING_EXTEND_0P=0x00 # index 0 only || previous (transfers only)
	COINSPARK_PACKING_EXTEND_PUBLIC=0x00 # this is public (messages only)
	COINSPARK_PACKING_EXTEND_1S=0x01 # index 1 || subsequent to previous (transfers only)
	COINSPARK_PACKING_EXTEND_0_1_BYTE=0x01 # starting at 0, 1 byte for count (messages only)
	COINSPARK_PACKING_EXTEND_1_0_BYTE=0x02 # 1 byte for single index, count is 1
	COINSPARK_PACKING_EXTEND_2_0_BYTES=0x03 # 2 bytes for single index, count is 1
	COINSPARK_PACKING_EXTEND_1_1_BYTES=0x04 # 1 byte for first index, 1 byte for count
	COINSPARK_PACKING_EXTEND_2_1_BYTES=0x05 # 2 bytes for first index, 1 byte for count
	COINSPARK_PACKING_EXTEND_2_2_BYTES=0x06 # 2 bytes for first index, 2 bytes for count
	COINSPARK_PACKING_EXTEND_ALL=0x07 # all inputs|outputs

	def isInteger(value)
		return (value.is_a? Integer) || ((value.is_a? Float) && (value%1==0))
	end
		
	def isBoolean(value)
		return (value==true) || (value==false)
	end

	def isString(value)
		return value.is_a? String
	end
		
	def writeSmallEndianUnsigned(value, bytes)
		return nil if value<0 # does not support negative values
	
		buffer=''
		
		for byte in 0...bytes
			buffer+=(value%256).round.to_i.chr
			value=(value/256).to_i
		end
		
		return (value>0) ? nil : buffer # if still something left, we didn't have enough bytes for representation
	end

	def readSmallEndianUnsigned(buffer, bytes)
		return nil if buffer.length<bytes

		value=0
		
		for bufferChar in buffer[0...bytes].reverse.chars
			value*=256
			value+=bufferChar.ord
		end
	
		return value
	end

	def unsignedToSmallEndianHex(value, bytes)
		string=''
		
		for byte in 0...bytes
			string+=sprintf("%02X", value%256)
			value/=256
		end
			
		return string
	end

	def base58ToInteger(base58Character)
		return COINSPARK_INTEGER_TO_BASE_58.index(base58Character)
	end
		
	def mantissaExponentToQty(mantissa, exponent)
		quantity=mantissa
	
		while (exponent>0)
			quantity*=10
			exponent-=1
		end
	
		return quantity
	end

	def qtyToMantissaExponent(quantity, rounding, mantissaMax, exponentMax)
		if rounding<0
			roundOffset=0
		elsif rounding>0
			roundOffset=9
		else
			roundOffset=4
		end
	
		exponent=0
	
		while quantity>mantissaMax
			quantity=((quantity+roundOffset)/10).to_i
			exponent+=1
		end
		
		exponent=[exponent, exponentMax].min

		return {
			'mantissa' => quantity.to_i,
			'exponent' => exponent.to_i,
			'qty' => mantissaExponentToQty(quantity, exponent)
		}
	end

	def getMinFeeBasis(outputsSatoshis, outputsRegular)
		smallestOutputSatoshis=COINSPARK_SATOSHI_QTY_MAX

		if outputsSatoshis.length==outputsRegular.length # if arrays different size, we can't use them
			for outputIndex in 0...outputsSatoshis.length
				if outputsRegular[outputIndex]
					smallestOutputSatoshis=[smallestOutputSatoshis, outputsSatoshis[outputIndex].to_i].min
				end
			end
		end

		return [COINSPARK_FEE_BASIS_MAX_SATOSHIS, smallestOutputSatoshis].min
	end

	def getLastRegularOutput(outputsRegular)
		lastRegularOutput=nil
		
		for outputIndex in 0...outputsRegular.length
			if outputsRegular[outputIndex]
				lastRegularOutput=(lastRegularOutput==nil) ? outputIndex : [outputIndex, lastRegularOutput].max
			end
		end
			
		return lastRegularOutput
	end

	def countNonLastRegularOutputs(outputsRegular)
		countRegularOutputs=0
		
		for outputRegular in outputsRegular
			countRegularOutputs+=1 if outputRegular
		end		
				
		return [countRegularOutputs-1, 0].max
	end

	def shrinkLowerDomainName(domainName)
		return nil if domainName.length==0
	
		domainName=domainName.downcase

		# Search for prefixes

		bestPrefixLen=-1
		
		for prefixIndex in 0...COINSPARK_DOMAIN_NAME_PREFIXES.length
			prefix=COINSPARK_DOMAIN_NAME_PREFIXES[prefixIndex]
			prefixLen=prefix.length
	
			if (prefixLen>bestPrefixLen) && (domainName[0...prefixLen]==prefix)
				bestPrefixIndex=prefixIndex
				bestPrefixLen=prefixLen
			end
		end

		domainName=domainName[bestPrefixLen..-1]
		domainNameLen=domainName.length

		# Search for suffixes

		bestSuffixLen=-1
		
		for suffixIndex in 0...COINSPARK_DOMAIN_NAME_SUFFIXES.length
			suffix=COINSPARK_DOMAIN_NAME_SUFFIXES[suffixIndex]
			suffixLen=suffix.length
	
			if (suffixLen>bestSuffixLen) && (domainName[domainNameLen-suffixLen..-1]==suffix)
				bestSuffixIndex=suffixIndex
				bestSuffixLen=suffixLen
			end
		end

		domainName=domainName[0...domainNameLen-bestSuffixLen]
		
		# Output and return

		packing=(((bestPrefixIndex<<COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT)&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)|
			(bestSuffixIndex&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK))

		return {
			'domainName' => domainName,
			'packing'=> packing
		}
	end

	def expandDomainName(domainName, packing)

		# Prefix

		prefixIndex=(packing&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)>>COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT
		return nil if prefixIndex>COINSPARK_DOMAIN_NAME_PREFIXES.length
			
		prefix=COINSPARK_DOMAIN_NAME_PREFIXES[prefixIndex]
	
		# Suffix

		suffixIndex=packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK
		return nil if suffixIndex>COINSPARK_DOMAIN_NAME_SUFFIXES.length
			
		suffix=COINSPARK_DOMAIN_NAME_SUFFIXES[suffixIndex]
	
		return prefix+domainName+suffix
	end

	def readIPv4Address(domainName)
		return nil if /[^0-9\.]/.match(domainName)
	
		octets=domainName.split('.')
		return nil if octets.length!=4
		
		for octet in octets
			return nil if (octet.length==0) || (octet.to_i>255)
		end
			
		return octets.map(&:to_i)
	end

	def encodeDomainPathTriplets(string)
		stringLen=string.length
		metadata=''

		for stringPos in 0...stringLen
			encodeValue=COINSPARK_DOMAIN_NAME_CHARS.index(string[stringPos])
			return nil if encodeValue==nil
			
			stringPosMod3=stringPos%3
			
			if stringPosMod3==0
				stringTriplet=encodeValue
			elsif stringPosMod3==1
				stringTriplet+=encodeValue*COINSPARK_DOMAIN_PATH_ENCODE_BASE
			elsif stringPosMod3==2
				stringTriplet+=encodeValue*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE
			end

			if (stringPosMod3==2) || (stringPos==(stringLen-1)) # write out 2 bytes if we've collected 3 chars, || if we're finishing
				written=writeSmallEndianUnsigned(stringTriplet, 2)
				return nil if written==nil
					
				metadata+=written
			end
		end

		return metadata
	end

	def decodeDomainPathTriplets(metadata, parts)
		startLength=metadata.length
		string=''
		stringPos=0
	
		while parts>0

			if (stringPos%3)==0
				stringTriplet=readSmallEndianUnsigned(metadata, 2)
				metadata=metadata[2..-1]
				return nil if stringTriplet==nil
					
				return nil if stringTriplet>=(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE)
					 # invalid value
			end

			stringPosMod3=stringPos%3
			
			if stringPosMod3==0
				decodeValue=stringTriplet%COINSPARK_DOMAIN_PATH_ENCODE_BASE
		
			elsif stringPosMod3==1
				decodeValue=(stringTriplet/COINSPARK_DOMAIN_PATH_ENCODE_BASE).to_i%COINSPARK_DOMAIN_PATH_ENCODE_BASE
				
			elsif stringPosMod3==2
				decodeValue=(stringTriplet/(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE)).to_i
			end

			decodeChar=COINSPARK_DOMAIN_NAME_CHARS[decodeValue]
			string=string+decodeChar
			stringPos+=1

			parts-=1 if (decodeChar==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR) || (decodeChar==COINSPARK_DOMAIN_PATH_FALSE_END_CHAR)
		end

		return {'string' => string, 'decodedChars' => startLength-metadata.length}
	end

	def encodeDomainAndOrPath(domainName, useHttps, pagePath, usePrefix, forMessages)
		metadata=''
		encodeString=''
	
		# Domain name

		if domainName!=nil
			octets=readIPv4Address(domainName)

			if octets!=nil
				
				if forMessages and (pagePath=='')
					packing=COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4_NO_PATH
					packing|=COINSPARK_DOMAIN_PACKING_IPv4_NO_PATH_PREFIX if usePrefix
						
					pagePath=nil # skip encoding the empty page path
				
				else
					packing=COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4
				end
					
				packing|=COINSPARK_DOMAIN_PACKING_IPv4_HTTPS if useHttps
					
				metadata=metadata+(
					packing.chr+
					octets[0].chr+
					octets[1].chr+
					octets[2].chr+
					octets[3].chr
				)

			else
				result=shrinkLowerDomainName(domainName)

				encodeString+=result['domainName']
				encodeString+=useHttps ? COINSPARK_DOMAIN_PATH_TRUE_END_CHAR : COINSPARK_DOMAIN_PATH_FALSE_END_CHAR
			
				metadata+=result['packing'].chr
			end
		end
	
		# Page path

		if pagePath!=nil
			encodeString+=pagePath
			encodeString+=usePrefix ? COINSPARK_DOMAIN_PATH_TRUE_END_CHAR : COINSPARK_DOMAIN_PATH_FALSE_END_CHAR
		end

		# Encode whatever is required as triplets

		if encodeString.length>0
			written=encodeDomainPathTriplets(encodeString)
			return nil if written==nil
				
			metadata+=written
		end

		return metadata
	end

	def decodeDomainAndOrPath(metadata, doDomainName, doPagePath, forMessages)
		startLength=metadata.length
		result={}
		metadataParts=0

		# Domain name

		if doDomainName
	
			# Get packing byte
	
			packingChar=metadata[0]
			metadata=metadata[1..-1]
			return nil if packingChar.length<1
			
			packing=packingChar.ord

			# Extract IP address if present
			
			packingSuffix=packing&COINSPARK_DOMAIN_PACKING_SUFFIX_MASK
			isIpAddress=((packingSuffix==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4) ||
				(forMessages && (packingSuffix==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4_NO_PATH)))
		
			if isIpAddress
				result['useHttps']=((packing&COINSPARK_DOMAIN_PACKING_IPv4_HTTPS)!=0)
	
				octetChars=metadata[0...4]
				metadata=metadata[4..-1]
				return nil if octetChars.length!=4
		
				result['domainName']=sprintf("%u.%u.%u.%u", octetChars[0].ord, octetChars[1].ord, octetChars[2].ord, octetChars[3].ord)
				
				if doPagePath && forMessages && (packingSuffix==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4_NO_PATH)
					result['pagePath']=''
					result['usePrefix']=((packing&COINSPARK_DOMAIN_PACKING_IPv4_NO_PATH_PREFIX)!=0)
					doPagePath=false # skip decoding the empty page path
				end

			else
				metadataParts+=1
			end
		end

		# Convert remaining metadata to string

		metadataParts+=1 if doPagePath

		if metadataParts>0
			decodeResult=decodeDomainPathTriplets(metadata, metadataParts)
			return nil if decodeResult==nil
			
			metadata=metadata[decodeResult['decodedChars']..-1]
			decodeString=decodeResult['string']

			# Extract domain name if IP address was not present
		
			if doDomainName && !isIpAddress
				endCharPos=(decodeString
					.tr(COINSPARK_DOMAIN_PATH_FALSE_END_CHAR, COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
					.index(COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
				)

				return nil if endCharPos==nil # should never happen
			
				result['domainName']=expandDomainName(decodeString[0...endCharPos], packing)
				return nil if result['domainName']==nil
				
				result['useHttps']=(decodeString[endCharPos]==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)

				decodeString=decodeString[endCharPos+1..-1]
			end

			# Extract page path

			if doPagePath
				endCharPos=(decodeString
					.tr(COINSPARK_DOMAIN_PATH_FALSE_END_CHAR, COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
					.index(COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
				)

				return nil if endCharPos==nil # should never happen
				
				result['pagePath']=decodeString[0...endCharPos]
				result['usePrefix']=(decodeString[endCharPos]==COINSPARK_DOMAIN_PATH_TRUE_END_CHAR)
				decodeString=decodeString[endCharPos+1..-1]
			end
		end

		# Finish and return
		
		result['decodedChars']=startLength-metadata.length

		return result
	end

	def normalizeIORanges(inRanges)
		countRanges=inRanges.length
		return inRanges if countRanges==0
		
		rangeUsed=[false]*countRanges
		outRanges=[]
		countRemoved=0
		lastRangeEnd=nil # need to define outside loop because of Ruby scope

		for orderIndex in 0...countRanges
			lowestRangeFirst=0
			lowestRangeIndex=-1

			for rangeIndex in 0...countRanges
				if !rangeUsed[rangeIndex]
					if (lowestRangeIndex==-1) || (inRanges[rangeIndex].first<lowestRangeFirst)
						lowestRangeFirst=inRanges[rangeIndex].first
						lowestRangeIndex=rangeIndex
					end
				end
			end

			if (orderIndex>0) && (inRanges[lowestRangeIndex].first<=lastRangeEnd) # we can combine two adjacent ranges
				countRemoved+=1
				thisRangeEnd=inRanges[lowestRangeIndex].first+inRanges[lowestRangeIndex].count
				outRanges[orderIndex-countRemoved].count=[lastRangeEnd, thisRangeEnd].max-outRanges[orderIndex-countRemoved].first
			else
				outRanges.push(inRanges[lowestRangeIndex].clone)
			end
	
			lastRangeEnd=outRanges[orderIndex-countRemoved].first+outRanges[orderIndex-countRemoved].count
			rangeUsed[lowestRangeIndex]=true
		end

		return outRanges
	end

	def getPackingOptions(previousRange, range, countInputsOutputs, forMessages)
		packingOptions={}
	
		firstZero=(range.first==0)
		firstByte=(range.first<=COINSPARK_UNSIGNED_BYTE_MAX)
		first2Bytes=(range.first<=COINSPARK_UNSIGNED_2_BYTES_MAX)
		countOne=(range.count==1)
		countByte=(range.count<=COINSPARK_UNSIGNED_BYTE_MAX)
		
		if forMessages
			packingOptions['_0P']=false
			packingOptions['_1S']=false # these two options not used for messages
			packingOptions['_0_1_BYTE']=firstZero && countByte
		
		else
			if previousRange
				packingOptions['_0P']=(range.first==previousRange.first) && (range.count==previousRange.count)
				packingOptions['_1S']=(range.first==(previousRange.first+previousRange.count)) && countOne
			else
				packingOptions['_0P']=firstZero && countOne
				packingOptions['_1S']=(range.first==1) && countOne
			end
				
			packingOptions['_0_1_BYTE']=false # this option not used for transfers
		end

		packingOptions['_1_0_BYTE']=firstByte && countOne
		packingOptions['_2_0_BYTES']=first2Bytes && countOne
		packingOptions['_1_1_BYTES']=firstByte && countByte
		packingOptions['_2_1_BYTES']=first2Bytes && countByte
		packingOptions['_2_2_BYTES']=first2Bytes && (range.count<=COINSPARK_UNSIGNED_2_BYTES_MAX)
		packingOptions['_ALL']=firstZero && (range.count>=countInputsOutputs.to_i)
	
		return packingOptions
	end

	def packingTypeToValues(packingType, previousRange, countInputOutputs)
		range=CoinSparkIORange.new
		
		if packingType=='_0P'
			if previousRange
				range.first=previousRange.first
				range.count=previousRange.count
			else
				range.first=0
				range.count=1
			end

		elsif packingType=='_1S'
			if previousRange
				range.first=previousRange.first+previousRange.count
			else
				range.first=1
			end

			range.count=1

		elsif packingType=='_0_1_BYTE'
			range.first=0
		
		elsif (packingType=='_1_0_BYTE') || (packingType=='_2_0_BYTES')
			range.count=1

		elsif packingType=='_ALL'
			range.first=0
			range.count=countInputOutputs.to_i
		end
	
		return range
	end

	def packingExtendAddByteCounts(packingExtend, firstBytes, countBytes, forMessages)
		if packingExtend==COINSPARK_PACKING_EXTEND_0_1_BYTE
			countBytes=1 if forMessages # otherwise it's really COINSPARK_PACKING_EXTEND_1S
		
		elsif packingExtend==COINSPARK_PACKING_EXTEND_1_0_BYTE
			firstBytes=1

		elsif packingExtend==COINSPARK_PACKING_EXTEND_2_0_BYTES
			firstBytes=2

		elsif packingExtend==COINSPARK_PACKING_EXTEND_1_1_BYTES
			firstBytes=1
			countBytes=1

		elsif packingExtend==COINSPARK_PACKING_EXTEND_2_1_BYTES
			firstBytes=2
			countBytes=1

		elsif packingExtend==COINSPARK_PACKING_EXTEND_2_2_BYTES
			firstBytes=2
			countBytes=2
		end
			
		return {'firstBytes' => firstBytes, 'countBytes' => countBytes}
	end
	
	def getPackingExtendMap
		 return [
			['_0P', COINSPARK_PACKING_EXTEND_0P],
			['_1S', COINSPARK_PACKING_EXTEND_1S],
			['_ALL', COINSPARK_PACKING_EXTEND_ALL],
			['_1_0_BYTE', COINSPARK_PACKING_EXTEND_1_0_BYTE],
			['_0_1_BYTE', COINSPARK_PACKING_EXTEND_0_1_BYTE],
			['_2_0_BYTES', COINSPARK_PACKING_EXTEND_2_0_BYTES],
			['_1_1_BYTES', COINSPARK_PACKING_EXTEND_1_1_BYTES],
			['_2_1_BYTES', COINSPARK_PACKING_EXTEND_2_1_BYTES],
			['_2_2_BYTES', COINSPARK_PACKING_EXTEND_2_2_BYTES]
		] # in order of preference (use array of arrays since hashes are unordered in old versions of Ruby)
	end

	def encodePackingExtend(packingOptions)
		packingExtendMap=getPackingExtendMap
	
		for (packingType, packingExtend) in packingExtendMap
			return packingExtend if packingOptions[packingType]
		end	
			
		return nil
	end

	def decodePackingExtend(packingExtend, forMessages)
		packingExtendMap=getPackingExtendMap
	
		for (packingType, packingExtendTest) in packingExtendMap
			if packingExtend==packingExtendTest
				return packingType if packingType!=(forMessages ? '_1S' : '_0_1_BYTE') # no _1S for messages, no _0_1_BYTE for transfers
			end
		end	
			
		return nil
	end

	def writeUnsignedField(bytes, source)
		return (bytes>0) ? writeSmallEndianUnsigned(source, bytes) : '' # will return nil on failure
	end

	def shiftReadUnsignedField(metadataArray, bytes, object, property)
		if bytes>0
			value=readSmallEndianUnsigned(metadataArray[0...bytes].join(''), bytes)
			metadataArray.shift(bytes)
			return false if value==nil
			
			object.instance_variable_set(property, value)
		end
		
		return true
	end

end


# CoinSparkAddress class for managing CoinSpark addresseses

class CoinSparkAddress < CoinSparkBase
	attr_accessor :bitcoinAddress
	attr_accessor :addressFlags
	attr_accessor :paymentRef
	
	COINSPARK_ADDRESS_PREFIX='s'
	COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE=10
	COINSPARK_ADDRESS_CHAR_INCREMENT=13
	
	def initialize
		clear
	end
	
	def clear
		@bitcoinAddress=''
		@addressFlags=0
		@paymentRef=CoinSparkPaymentRef.new
	end
	
	def toString
		flagsToStrings={
			COINSPARK_ADDRESS_FLAG_ASSETS => "assets",
			COINSPARK_ADDRESS_FLAG_PAYMENT_REFS => "payment references",
			COINSPARK_ADDRESS_FLAG_TEXT_MESSAGES => "text messages",
			COINSPARK_ADDRESS_FLAG_FILE_MESSAGES => "file messages"
		}
		
		buffer="COINSPARK ADDRESS\n"
		buffer+=sprintf("  Bitcoin address: %s\n", @bitcoinAddress)
		buffer+=sprintf("    Address flags: %d", @addressFlags)
		
		flagOutput=false
		
		flagsToStrings.each_pair do |flag, string|
			if (@addressFlags & flag)!=0
				buffer+=(flagOutput ? ", " : " [")+string
				flagOutput=true
			end
		end
		
		buffer+=(flagOutput ? "]" : "")+"\n"
		
		buffer+=sprintf("Payment reference: %.0f\n", @paymentRef.ref)
		buffer+="END COINSPARK ADDRESS\n\n"
		
		return buffer
	end
	
	def isValid
		return false if (!isString(@bitcoinAddress)) || (@bitcoinAddress.length==0)
		
		return false if (!isInteger(@addressFlags)) || ((@addressFlags&COINSPARK_ADDRESS_FLAG_MASK)!=@addressFlags)
		
		return @paymentRef.isValid
	end
	
	def match(otherAddress)
		return ((@bitcoinAddress==otherAddress.bitcoinAddress) &&
			@addressFlags==otherAddress.addressFlags &&
			@paymentRef.match(otherAddress.paymentRef))
	end
	
	def encode
		return nil if !isValid
		
		stringBase58=[0, 0]
		
		# Build up extra data for address flags
		
		addressFlagChars=0
		testAddressFlags=@addressFlags
		
		while testAddressFlags>0
			stringBase58.push(testAddressFlags.to_i%58)
			testAddressFlags=(testAddressFlags.to_i/58) # treat as an integer
			addressFlagChars+=1
		end
		
		# Build up extra data for payment reference
		
		paymentRefChars=0
		testPaymentRef=@paymentRef.ref
		
		while testPaymentRef>0
			stringBase58.push(testPaymentRef.to_i%58)
			testPaymentRef=(testPaymentRef.to_i/58)
			paymentRefChars+=1
		end
		
		# Calculate and encode extra length
		
		extraDataChars=addressFlagChars+paymentRefChars
		bitcoinAddressLen=@bitcoinAddress.length
		stringLen=bitcoinAddressLen+2+extraDataChars
		
		stringBase58[1]=addressFlagChars*COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE+paymentRefChars
		
		# Convert the bitcoin address
		
		for charIndex in 0...bitcoinAddressLen
			charValue=base58ToInteger(@bitcoinAddress[charIndex])
			return nil if charValue==nil # invalid base58 character
				
			charValue+=COINSPARK_ADDRESS_CHAR_INCREMENT
			
			charValue+=stringBase58[2+charIndex%extraDataChars] if extraDataChars>0
				
			stringBase58.push(charValue%58)
		end
		
		# Obfuscate first half of address using second half to prevent common prefixes
		
		halfLength=(stringLen/2.0).ceil.to_i
		for charIndex in 1...halfLength # exclude first character
			stringBase58[charIndex]=(stringBase58[charIndex]+stringBase58[stringLen-charIndex])%58
		end
		
		# Convert to base 58 and add prefix
		
		string=COINSPARK_ADDRESS_PREFIX
		for charValue in stringBase58.slice(1...stringLen)
			string+=COINSPARK_INTEGER_TO_BASE_58[charValue]
		end
		
		return string
	end

	def decode(string)
	
		# Check for basic validity
		
		stringLen=string.length
		return false if stringLen<2

		return false if string[0]!=COINSPARK_ADDRESS_PREFIX
			
		# Convert from base 58
	
		stringBase58=[0]
		for stringChar in string[1..-1].chars # exclude first character
			charValue=base58ToInteger(stringChar)
			return false if charValue==nil
			stringBase58.push(charValue)
		end
			
		# De-obfuscate first half of address using second half
	
		halfLength=(stringLen/2.0).ceil.to_i
		for charIndex in 1...halfLength # exclude first character
			stringBase58[charIndex]=(stringBase58[charIndex]+58-stringBase58[stringLen-charIndex])%58
		end			
	
		# Get length of extra data
	
		charValue=stringBase58[1]
		addressFlagChars=charValue/COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE
		paymentRefChars=charValue%COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE
		extraDataChars=addressFlagChars+paymentRefChars
		
		return false if stringLen<(2+extraDataChars)
			
		bitcoinAddressLen=stringLen-2-extraDataChars
		
		# Read the extra data for address flags
	
		@addressFlags=0
		multiplier=1
		
		for charValue in stringBase58[2...2+addressFlagChars]
			@addressFlags+=charValue*multiplier
			multiplier*=58
		end			
	
		# Read the extra data for payment reference
	
		@paymentRef.ref=0
		multiplier=1
		
		for charValue in stringBase58[2+addressFlagChars...2+extraDataChars]
			@paymentRef.ref+=charValue*multiplier
			multiplier*=58
		end
			
		# Convert the bitcoin address
		
		@bitcoinAddress=''
		
		for charIndex in 0...bitcoinAddressLen
			charValue=stringBase58[2+extraDataChars+charIndex]
			charValue+=58*2-COINSPARK_ADDRESS_CHAR_INCREMENT # avoid worrying about the result of modulo on negative numbers
			
			charValue-=stringBase58[2+charIndex%extraDataChars] if extraDataChars>0
				
			@bitcoinAddress+=COINSPARK_INTEGER_TO_BASE_58[charValue%58]
		end
		
		return isValid
	end

end


# CoinSparkGenesis class for managing asset genesis metadata

class CoinSparkGenesis < CoinSparkBase
	attr_accessor :qtyMantissa
	attr_accessor :qtyExponent
	attr_accessor :chargeFlatMantissa
	attr_accessor :chargeFlatExponent
	attr_accessor :chargeBasisPoints
	attr_accessor :useHttps
	attr_accessor :domainName
	attr_accessor :usePrefix
	attr_accessor :pagePath
	attr_accessor :assetHash
	attr_accessor :assetHashLen
	
	COINSPARK_GENESIS_QTY_FLAGS_LENGTH=2
	COINSPARK_GENESIS_QTY_MASK=0x3FFF
	COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE=1001
	COINSPARK_GENESIS_FLAG_CHARGE_FLAT=0x4000
	COINSPARK_GENESIS_FLAG_CHARGE_BPS=0x8000
	COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE=101
	COINSPARK_GENESIS_CHARGE_FLAT_LENGTH=1
	COINSPARK_GENESIS_CHARGE_BPS_LENGTH=1
	
	def initialize
		clear
	end

	def clear
		@qtyMantissa=0
		@qtyExponent=0
		@chargeFlatMantissa=0
		@chargeFlatExponent=0
		@chargeBasisPoints=0
		@useHttps=false
		@domainName=''
		@usePrefix=true
		@pagePath=''
		@assetHash=''
		@assetHashLen=0
	end
		
	def toString
		quantity=getQty
		quantityEncoded=(@qtyExponent*COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+@qtyMantissa)&COINSPARK_GENESIS_QTY_MASK
		chargeFlat=getChargeFlat
		chargeFlatEncoded=@chargeFlatExponent*COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+@chargeFlatMantissa
		domainPathMetadata=encodeDomainAndOrPath(@domainName, @useHttps, @pagePath, @usePrefix, false)
		
		buffer="COINSPARK GENESIS\n"
		buffer+=sprintf("   Quantity mantissa: %d\n", @qtyMantissa)
		buffer+=sprintf("   Quantity exponent: %d\n", @qtyExponent)
		buffer+=sprintf("    Quantity encoded: %d (small endian hex %s)\n", quantityEncoded,
			unsignedToSmallEndianHex(quantityEncoded, COINSPARK_GENESIS_QTY_FLAGS_LENGTH) )
		buffer+=sprintf("      Quantity value: %.0f\n", quantity)
		buffer+=sprintf("Flat charge mantissa: %d\n", @chargeFlatMantissa)
		buffer+=sprintf("Flat charge exponent: %d\n", @chargeFlatExponent)
		buffer+=sprintf(" Flat charge encoded: %d (small endian hex %s)\n", chargeFlatEncoded,
			unsignedToSmallEndianHex(chargeFlatEncoded, COINSPARK_GENESIS_CHARGE_FLAT_LENGTH))
		buffer+=sprintf("   Flat charge value: %.0f\n", chargeFlat)
		buffer+=sprintf(" Basis points charge: %d (hex %s)\n", @chargeBasisPoints,
			unsignedToSmallEndianHex(@chargeBasisPoints, COINSPARK_GENESIS_CHARGE_BPS_LENGTH))
		buffer+=sprintf("           Asset URL: %s://%s/%s%s/ (length %d+%d encoded %s length %d)\n",
			@useHttps ? 'https' : 'http', @domainName,
			@usePrefix ? "coinspark/" : "", (@pagePath.length>0) ? @pagePath : "[spent-txid]",
			@domainName.length, @pagePath.length,
			domainPathMetadata.unpack('H*')[0].upcase, domainPathMetadata.length
		)
		buffer+=sprintf("          Asset hash: %s (length %d)\n", @assetHash[0...@assetHashLen].unpack('H*')[0].upcase, @assetHashLen)
		buffer+="END COINSPARK GENESIS\n\n"
		
		return buffer
	end
	
	def isValid
		return false if !(
			isInteger(@qtyMantissa) &&
			isInteger(@qtyExponent) &&
			isInteger(@chargeFlatMantissa) &&
			isInteger(@chargeFlatExponent) &&
			isInteger(@chargeBasisPoints) &&
			isBoolean(@useHttps) &&
			isString(@domainName) &&
			isBoolean(@usePrefix) &&
			isString(@pagePath) &&
			isString(@assetHash) &&
			isInteger(@assetHashLen)
		)
		
		return false if (@qtyMantissa<COINSPARK_GENESIS_QTY_MANTISSA_MIN) || (@qtyMantissa>COINSPARK_GENESIS_QTY_MANTISSA_MAX)
			
		return false if (@qtyExponent<COINSPARK_GENESIS_QTY_EXPONENT_MIN) || (@qtyExponent>COINSPARK_GENESIS_QTY_EXPONENT_MAX)
			
		return false if (@chargeFlatExponent<COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN) || (@chargeFlatExponent>COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX)
			
		return false if @chargeFlatMantissa<COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN
			
		return false if @chargeFlatMantissa > ((@chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) ? COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX : COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX)
			
		return false if (@chargeBasisPoints<COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN) || (@chargeBasisPoints>COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX)
			
		return false if @domainName.length>COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN
			
		return false if @pagePath.length>COINSPARK_GENESIS_PAGE_PATH_MAX_LEN

		return false if @assetHash.length<@assetHashLen # check we have at least as much data as specified by @assetHashLen
		
		return false if (@assetHashLen<COINSPARK_GENESIS_HASH_MIN_LEN) || (@assetHashLen>COINSPARK_GENESIS_HASH_MAX_LEN)
			
		return true
	end
	
	def match(otherGenesis, strict)
		hashCompareLen=[@assetHashLen, otherGenesis.assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN].min
		
		if strict
			floatQuantitiesMatch=(
				(@qtyMantissa==otherGenesis.qtyMantissa) && (@qtyExponent==otherGenesis.qtyExponent) &&
				(@chargeFlatMantissa==otherGenesis.chargeFlatMantissa) && (@chargeFlatExponent==otherGenesis.chargeFlatExponent)
			)
		else
			floatQuantitiesMatch=(getQty==otherGenesis.getQty) && (getChargeFlat==otherGenesis.getChargeFlat)
		end
		
		return (
			floatQuantitiesMatch && (@chargeBasisPoints==otherGenesis.chargeBasisPoints) &&
			(@useHttps==otherGenesis.useHttps) &&
			(@domainName.downcase==otherGenesis.domainName.downcase) &&
			(@usePrefix==otherGenesis.usePrefix) &&
			(@pagePath.downcase==otherGenesis.pagePath.downcase) &&
			(@assetHash[0...hashCompareLen]==otherGenesis.assetHash[0...hashCompareLen])
		)
	end
	
	def getQty
		return mantissaExponentToQty(@qtyMantissa, @qtyExponent)
	end
	
	def setQty(desiredQty, rounding)
		result=qtyToMantissaExponent(desiredQty, rounding, COINSPARK_GENESIS_QTY_MANTISSA_MAX,
			COINSPARK_GENESIS_QTY_EXPONENT_MAX)
		@qtyMantissa=result['mantissa']
		@qtyExponent=result['exponent']
			
		return getQty
	end
	
	def getChargeFlat
		return mantissaExponentToQty(@chargeFlatMantissa, @chargeFlatExponent)
	end
	
	def setChargeFlat(desiredChargeFlat, rounding)
		result=qtyToMantissaExponent(desiredChargeFlat, rounding, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX,
			COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX)
		@chargeFlatMantissa=result['mantissa']
		@chargeFlatExponent=result['exponent']
			
		if @chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX
			@chargeFlatMantissa=[@chargeFlatMantissa, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX].min
		end
			
		return getChargeFlat
	end
	
	def calcCharge(qtyGross)
		charge=getChargeFlat+((qtyGross*@chargeBasisPoints+5000)/10000).to_i # rounds to nearest
		
		return [qtyGross, charge].min
	end
	
	def calcNet(qtyGross)
		return qtyGross-calcCharge(qtyGross)
	end
	
	def calcGross(qtyNet)
		qtyNet=qtyNet.to_f
		
		return 0 if qtyNet<=0 # no point getting past charges if we end up with zero anyway
			
		lowerGross=(((qtyNet+getChargeFlat)*10000)/(10000-@chargeBasisPoints)).to_i # divides rounding down
		
		return (calcNet(lowerGross)>=qtyNet) ? lowerGross : (lowerGross+1)
	end
	
	def calcHashLen(metadataMaxLen)
		assetHashLen=metadataMaxLen-COINSPARK_METADATA_IDENTIFIER_LEN-1-COINSPARK_GENESIS_QTY_FLAGS_LENGTH
		
		assetHashLen-=COINSPARK_GENESIS_CHARGE_FLAT_LENGTH if @chargeFlatMantissa>0
			
		assetHashLen-=COINSPARK_GENESIS_CHARGE_BPS_LENGTH if @chargeBasisPoints>0
			
		domainPathLen=@pagePath.length+1
			
		if readIPv4Address(@domainName)
			assetHashLen-=5 # packing && IP octets
		else
			assetHashLen-=1 # packing
			domainPathLen+=shrinkLowerDomainName(@domainName)['domainName'].length+1
		end
		
		assetHashLen-=2*((domainPathLen+2)/3).to_i # uses integer arithmetic
		
		return [assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN].min
	end

	def encode(metadataMaxLen)
		return nil if !isValid

		# 4-character identifier
			
		metadata=COINSPARK_METADATA_IDENTIFIER+COINSPARK_GENESIS_PREFIX

		# Quantity mantissa and exponent
	
		quantityEncoded=(@qtyExponent*COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE+@qtyMantissa)&COINSPARK_GENESIS_QTY_MASK
		quantityEncoded|=COINSPARK_GENESIS_FLAG_CHARGE_FLAT if @chargeFlatMantissa>0
		quantityEncoded|=COINSPARK_GENESIS_FLAG_CHARGE_BPS if @chargeBasisPoints>0
		
		written=writeSmallEndianUnsigned(quantityEncoded, COINSPARK_GENESIS_QTY_FLAGS_LENGTH)
		return nil if written==nil
		
		metadata+=written
		
		# Charges - flat and basis points
	
		if (quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_FLAT)!=0
			chargeEncoded=@chargeFlatExponent*COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+@chargeFlatMantissa
		
			written=writeSmallEndianUnsigned(chargeEncoded, COINSPARK_GENESIS_CHARGE_FLAT_LENGTH)
			return nil if written==nil
				
			metadata+=written
		end

		if (quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_BPS)!=0
			written=writeSmallEndianUnsigned(@chargeBasisPoints, COINSPARK_GENESIS_CHARGE_BPS_LENGTH)
			return nil if written==nil
				
			metadata+=written
		end
		
		# Domain name and page path
	
		written=encodeDomainAndOrPath(@domainName, @useHttps, @pagePath, @usePrefix, false)
		return nil if written==nil

		metadata+=written

		# Asset hash
	
		metadata+=@assetHash[0...@assetHashLen]
		
		# Check the total length is within the specified limit
	
		return nil if metadata.length>metadataMaxLen	
			
		# Return what we created
	
		return metadata
	end
	
	def decode(metadata)
		metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_GENESIS_PREFIX)
		return false if metadata==nil
		
		# Quantity mantissa and exponent
	
		quantityEncoded=readSmallEndianUnsigned(metadata, COINSPARK_GENESIS_QTY_FLAGS_LENGTH)
		metadata=metadata[COINSPARK_GENESIS_QTY_FLAGS_LENGTH..-1]
		return false if quantityEncoded==nil
			
		@qtyMantissa=(quantityEncoded&COINSPARK_GENESIS_QTY_MASK)%COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE
		@qtyExponent=(quantityEncoded&COINSPARK_GENESIS_QTY_MASK)/COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE
	
		# Charges - flat and basis points
	
		if (quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_FLAT)!=0
			chargeEncoded=readSmallEndianUnsigned(metadata, COINSPARK_GENESIS_CHARGE_FLAT_LENGTH)
			metadata=metadata[COINSPARK_GENESIS_CHARGE_FLAT_LENGTH..-1]
			
			@chargeFlatMantissa=chargeEncoded%COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE
			@chargeFlatExponent=chargeEncoded/COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE
		else
			@chargeFlatMantissa=0
			@chargeFlatExponent=0
		end
		
		if (quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_BPS)!=0
			@chargeBasisPoints=readSmallEndianUnsigned(metadata, COINSPARK_GENESIS_CHARGE_BPS_LENGTH)
			metadata=metadata[COINSPARK_GENESIS_CHARGE_BPS_LENGTH..-1]
		else
			@chargeBasisPoints=0
		end
	
		# Domain name && page path
		
		decodedDomainPath=decodeDomainAndOrPath(metadata, true, true, false)
		return false if decodedDomainPath==nil

		metadata=metadata[decodedDomainPath['decodedChars']..-1]
		@useHttps=decodedDomainPath['useHttps']
		@domainName=decodedDomainPath['domainName']
		@usePrefix=decodedDomainPath['usePrefix']
		@pagePath=decodedDomainPath['pagePath']
		
		# Asset hash
	
		@assetHashLen=[metadata.length, COINSPARK_GENESIS_HASH_MAX_LEN].min
		@assetHash=metadata[0...@assetHashLen]
		
		# Return validity
	
		return isValid
	end
	
	def calcMinFee(outputsSatoshis, outputsRegular)
		return COINSPARK_SATOSHI_QTY_MAX if outputsSatoshis.length!=outputsRegular.length # these two arrays must be the same size

		return countNonLastRegularOutputs(outputsRegular)*getMinFeeBasis(outputsSatoshis, outputsRegular)
	end
	
	def apply(outputsRegular)
		countOutputs=outputsRegular.length
		lastRegularOutput=getLastRegularOutput(outputsRegular)
		divideOutputs=countNonLastRegularOutputs(outputsRegular)
		genesisQty=getQty
		
		if divideOutputs==0
			qtyPerOutput=0
		else
			qtyPerOutput=(genesisQty/divideOutputs).to_i # rounds down
		end
			
		extraFirstOutput=genesisQty-qtyPerOutput*divideOutputs
		outputBalances=[0]*countOutputs
		
		for outputIndex in 0...countOutputs
			if outputsRegular[outputIndex] && (outputIndex!=lastRegularOutput)
				outputBalances[outputIndex]=qtyPerOutput+extraFirstOutput
				extraFirstOutput=0 # so it will only contribute to the first
			end
		end
		
		return outputBalances
	end
	
	def calcAssetURL(firstSpentTxID, firstSpentVout)
		firstSpentTxIdPart=(firstSpentTxID+firstSpentTxID).slice(firstSpentVout.to_i%64, 16)
		
		return (
			(@useHttps ? 'https' : 'http')+
			'://'+@domainName+'/'+
			(@usePrefix ? 'coinspark/' : '')+
			((@pagePath.length>0) ? @pagePath : firstSpentTxIdPart)+'/'
		).downcase
	end

end


# CoinSparkAssetRef class for managing asset references

class CoinSparkAssetRef < CoinSparkBase
	attr_accessor :blockNum
	attr_accessor :txOffset
	attr_accessor :txIDPrefix
	
	def initialize
		clear
	end

	def clear
		@blockNum=0
		@txOffset=0
		@txIDPrefix='00'*COINSPARK_ASSETREF_TXID_PREFIX_LEN
	end
		
	def toString
		return toStringInner(true)
	end
	
	def isValid
		if @blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE
			return false if (@blockNum<0) || (@blockNum>COINSPARK_ASSETREF_BLOCK_NUM_MAX) || !isInteger(@blockNum)
				
			return false if (@txOffset<0) || (@txOffset>COINSPARK_ASSETREF_TX_OFFSET_MAX) || !isInteger(@txOffset)
				
			return false if (!isString(@txIDPrefix)) || (@txIDPrefix.length!=(2*COINSPARK_ASSETREF_TXID_PREFIX_LEN))
				
			return false if @txIDPrefix[/\H/] # finds a non-hex digit
		end
		
		return true
	end
	
	def match(otherAssetRef)
		return ( (@txIDPrefix.downcase==otherAssetRef.txIDPrefix.downcase) &&
			(@txOffset == otherAssetRef.txOffset) && (@blockNum == otherAssetRef.blockNum) )
	end
	
	def encode
		return nil if !isValid
			
		txIDPrefixInteger=256*@txIDPrefix[2...4].to_i(16)+@txIDPrefix[0...2].to_i(16)
		
		return @blockNum.to_s+'-'+@txOffset.to_s+'-'+txIDPrefixInteger.to_s
	end
	
	def decode(string)
		return false if string[/[^0-9\-]/]
			
		parts=string.split('-').map(&:to_i)
		
		return false if (parts.length!=3) || (parts[2]>0xFFFF)

		@blockNum=parts[0]
		@txOffset=parts[1]
		@txIDPrefix=sprintf("%02X%02X", parts[2]%256, parts[2]/256)
		
		return isValid
	end	
		
	def toStringInner(headers)
		buffer=headers ? "COINSPARK ASSET REFERENCE\n" : ""

		buffer+=sprintf("Genesis block index: %.0f (small endian hex %s)\n", @blockNum, unsignedToSmallEndianHex(@blockNum, 4))
		buffer+=sprintf(" Genesis txn offset: %.0f (small endian hex %s)\n", @txOffset, unsignedToSmallEndianHex(@txOffset, 4))
		buffer+=sprintf("Genesis txid prefix: %s\n", @txIDPrefix.upcase)
		
		buffer+="END COINSPARK ASSET REFERENCE\n\n" if headers
			
		return buffer
	end
		
	def compare(otherAssetRef)
		# -1 if this<otherAssetRef, 1 if otherAssetRef>this, 0 otherwise
	
		if @blockNum!=otherAssetRef.blockNum
			return (@blockNum<otherAssetRef.blockNum) ? -1 : 1
		elsif @blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE # in this case don't compare other fields
			return 0
		elsif @txOffset!=otherAssetRef.txOffset
			return (@txOffset<otherAssetRef.txOffset) ? -1 : 1
		else
			thisTxIDPrefixLower=@txIDPrefix[0...2*COINSPARK_ASSETREF_TXID_PREFIX_LEN].downcase
			otherTxIDPrefixLower=otherAssetRef.txIDPrefix[0...2*COINSPARK_ASSETREF_TXID_PREFIX_LEN].downcase
		
			if thisTxIDPrefixLower!=otherTxIDPrefixLower # comparing hex gives same order as comparing bytes
				return (thisTxIDPrefixLower<otherTxIDPrefixLower) ? -1 : 1
			else
				return 0
			end
		end
	end

end


# CoinSparkTransfer class for managing individual asset transfer metadata

class CoinSparkTransfer < CoinSparkBase
	attr_accessor :assetRef
	attr_accessor :inputs
	attr_accessor :outputs
	attr_accessor :qtyPerOutput

	COINSPARK_PACKING_GENESIS_MASK=0xC0
	COINSPARK_PACKING_GENESIS_PREV=0x00
	COINSPARK_PACKING_GENESIS_3_3_BYTES=0x40 # 3 bytes for block index, 3 for txn offset
	COINSPARK_PACKING_GENESIS_3_4_BYTES=0x80 # 3 bytes for block index, 4 for txn offset
	COINSPARK_PACKING_GENESIS_4_4_BYTES=0xC0 # 4 bytes for block index, 4 for txn offset

	COINSPARK_PACKING_INDICES_MASK=0x38
	COINSPARK_PACKING_INDICES_0P_0P=0x00 # input 0 only || previous, output 0 only || previous
	COINSPARK_PACKING_INDICES_0P_1S=0x08 # input 0 only || previous, output 1 only || subsequent single
	COINSPARK_PACKING_INDICES_0P_ALL=0x10 # input 0 only || previous, all outputs
	COINSPARK_PACKING_INDICES_1S_0P=0x18 # input 1 only || subsequent single, output 0 only || previous
	COINSPARK_PACKING_INDICES_ALL_0P=0x20 # all inputs, output 0 only || previous
	COINSPARK_PACKING_INDICES_ALL_1S=0x28 # all inputs, output 1 only || subsequent single
	COINSPARK_PACKING_INDICES_ALL_ALL=0x30 # all inputs, all outputs
	COINSPARK_PACKING_INDICES_EXTEND=0x38 # use second byte for more extensive information

	COINSPARK_PACKING_EXTEND_INPUTS_SHIFT=3
	COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT=0

	COINSPARK_PACKING_QUANTITY_MASK=0x07
	COINSPARK_PACKING_QUANTITY_1P=0x00 # quantity=1 || previous
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

	def initialize
		clear
	end

	def clear
		@assetRef=CoinSparkAssetRef.new
		@inputs=CoinSparkIORange.new
		@outputs=CoinSparkIORange.new
		@qtyPerOutput=0
	end

	def toString
		return toStringInner(true)
	end

	def isValid
		return false if !(@assetRef.isValid && @inputs.isValid && @outputs.isValid)
			
		return false if (@qtyPerOutput<0) || (@qtyPerOutput>COINSPARK_ASSET_QTY_MAX) || !isInteger(@qtyPerOutput)
		
		return true
	end

	def match(otherTransfer)
		if @assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE
			return ( (otherTransfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) &&
				@inputs.match(otherTransfer.inputs) && (@outputs.first==otherTransfer.outputs.first) )

		else
			return (@assetRef.match(otherTransfer.assetRef) &&
				@inputs.match(otherTransfer.inputs) &&
				@outputs.match(otherTransfer.outputs) &&
				@qtyPerOutput==otherTransfer.qtyPerOutput)
		end
	end

	def encode(previousTransfer, metadataMaxLen, countInputs, countOutputs)
		return nil if !isValid
	
		packing=0
		packingExtend=0
		isDefaultRoute=(@assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
	
		# Packing for genesis reference
	
		if isDefaultRoute
			return nil if previousTransfer && (previousTransfer.assetRef.blockNum!=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
				# default route transfers have to come at the start
			
			packing|=COINSPARK_PACKING_GENESIS_PREV
	
		else
			if previousTransfer && @assetRef.match(previousTransfer.assetRef)
				packing|=COINSPARK_PACKING_GENESIS_PREV
		
			elsif @assetRef.blockNum <= COINSPARK_UNSIGNED_3_BYTES_MAX
				if @assetRef.txOffset <= COINSPARK_UNSIGNED_3_BYTES_MAX
					packing|=COINSPARK_PACKING_GENESIS_3_3_BYTES
				elsif @assetRef.txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX
					packing|=COINSPARK_PACKING_GENESIS_3_4_BYTES
				else
					return nil
				end

			elsif (@assetRef.blockNum <= COINSPARK_UNSIGNED_4_BYTES_MAX) && (@assetRef.txOffset <= COINSPARK_UNSIGNED_4_BYTES_MAX)
				packing|=COINSPARK_PACKING_GENESIS_4_4_BYTES
		
			else
				return nil
			end
		end
		
		# Packing for input && output indices

		inputPackingOptions=getPackingOptions(previousTransfer ? previousTransfer.inputs : nil, @inputs, countInputs, false)
		outputPackingOptions=getPackingOptions(previousTransfer ? previousTransfer.outputs : nil, @outputs, countOutputs, false)
	
		if inputPackingOptions['_0P'] && outputPackingOptions['_0P']
			packing|=COINSPARK_PACKING_INDICES_0P_0P
		elsif inputPackingOptions['_0P'] && outputPackingOptions['_1S']
			packing|=COINSPARK_PACKING_INDICES_0P_1S
		elsif inputPackingOptions['_0P'] && outputPackingOptions['_ALL']
			packing|=COINSPARK_PACKING_INDICES_0P_ALL
		elsif inputPackingOptions['_1S'] && outputPackingOptions['_0P']
			packing|=COINSPARK_PACKING_INDICES_1S_0P
		elsif inputPackingOptions['_ALL'] && outputPackingOptions['_0P']
			packing|=COINSPARK_PACKING_INDICES_ALL_0P
		elsif inputPackingOptions['_ALL'] && outputPackingOptions['_1S']
			packing|=COINSPARK_PACKING_INDICES_ALL_1S
		elsif inputPackingOptions['_ALL'] && outputPackingOptions['_ALL']
			packing|=COINSPARK_PACKING_INDICES_ALL_ALL

		else # we need the second (extended) packing byte
			packing|=COINSPARK_PACKING_INDICES_EXTEND

			packingExtendInput=encodePackingExtend(inputPackingOptions)
			packingExtendOutput=encodePackingExtend(outputPackingOptions)

			return nil if (packingExtendInput==nil) || (packingExtendOutput==nil)
			
			packingExtend=(packingExtendInput << COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) | (packingExtendOutput << COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT)
		end
	
		# Packing for quantity
	
		encodeQuantity=@qtyPerOutput
   
		if @qtyPerOutput==(previousTransfer ? previousTransfer.qtyPerOutput : 1)
			packing|=COINSPARK_PACKING_QUANTITY_1P
		elsif @qtyPerOutput>=COINSPARK_ASSET_QTY_MAX
			packing|=COINSPARK_PACKING_QUANTITY_MAX
		elsif @qtyPerOutput<=COINSPARK_UNSIGNED_BYTE_MAX
			packing|=COINSPARK_PACKING_QUANTITY_1_BYTE
		elsif @qtyPerOutput<=COINSPARK_UNSIGNED_2_BYTES_MAX
			packing|=COINSPARK_PACKING_QUANTITY_2_BYTES
		else
			result=qtyToMantissaExponent(@qtyPerOutput, 0,
				COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX, COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX)
			
			if result['qty']==@qtyPerOutput
				packing|=COINSPARK_PACKING_QUANTITY_FLOAT
				encodeQuantity=(result['exponent']*COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+result['mantissa'])&COINSPARK_TRANSFER_QTY_FLOAT_MASK
		
			elsif @qtyPerOutput<=COINSPARK_UNSIGNED_3_BYTES_MAX
				packing|=COINSPARK_PACKING_QUANTITY_3_BYTES
			elsif @qtyPerOutput<=COINSPARK_UNSIGNED_4_BYTES_MAX
				packing|=COINSPARK_PACKING_QUANTITY_4_BYTES
			else
				packing|=COINSPARK_PACKING_QUANTITY_6_BYTES
			end
		end
		
		# Write out the actual data

		counts=packingToByteCounts(packing, packingExtend)
	
		metadata=packing.chr
	
		if (packing & COINSPARK_PACKING_INDICES_MASK) == COINSPARK_PACKING_INDICES_EXTEND
			metadata+=packingExtend.chr
		end
	
		written_array=[
			writeUnsignedField(counts['blockNumBytes'], @assetRef.blockNum),
			writeUnsignedField(counts['txOffsetBytes'], @assetRef.txOffset),
			([@assetRef.txIDPrefix].pack('H*')+("\x00"*counts['txIDPrefixBytes']))[0...counts['txIDPrefixBytes']], # ensure right length
			writeUnsignedField(counts['firstInputBytes'], @inputs.first),
			writeUnsignedField(counts['countInputsBytes'], @inputs.count),
			writeUnsignedField(counts['firstOutputBytes'], @outputs.first),
			writeUnsignedField(counts['countOutputsBytes'], @outputs.count),
			writeUnsignedField(counts['quantityBytes'], encodeQuantity)
		]
		
		for written in written_array
			return nil if written==nil
			
			metadata+=written
		end
			
		# Check the total length is within the specified limit

		return nil if metadata.length>metadataMaxLen
		
		# Return what we created

		return metadata			
	end

	def decode(metadata, previousTransfer, countInputs, countOutputs)
		startLength=metadata.length
	
		# Extract packing

		packing=readSmallEndianUnsigned(metadata, 1)
		metadata=metadata[1..-1]
		return 0 if packing==nil

		packingExtend=0
		
		# Packing for genesis reference

		if (packing & COINSPARK_PACKING_GENESIS_MASK)==COINSPARK_PACKING_GENESIS_PREV
			if previousTransfer
				@assetRef=previousTransfer.assetRef
			
			else # it's for a default route
				@assetRef.blockNum=COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE
				@assetRef.txOffset=0
				@assetRef.txIDPrefix="00"*COINSPARK_ASSETREF_TXID_PREFIX_LEN
			end
		end

		# Packing for input && output indices

		if (packing & COINSPARK_PACKING_INDICES_MASK) == COINSPARK_PACKING_INDICES_EXTEND # we're using second packing metadata byte
			packingExtend=readSmallEndianUnsigned(metadata, 1)
			metadata=metadata[1..-1]
			return 0 if packingExtend==nil
			
			inputPackingType=decodePackingExtend((packingExtend >> COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK, false)
			outputPackingType=decodePackingExtend((packingExtend >> COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK, false)

			return 0 if (inputPackingType==nil) || (outputPackingType==nil)

		else # not using second packing metadata byte

			packingIndices=packing & COINSPARK_PACKING_INDICES_MASK
			
			# input packing
			
			if (
				(packingIndices==COINSPARK_PACKING_INDICES_0P_0P) ||
				(packingIndices==COINSPARK_PACKING_INDICES_0P_1S) ||
				(packingIndices==COINSPARK_PACKING_INDICES_0P_ALL)
			)
				inputPackingType='_0P'
				
			elsif packingIndices==COINSPARK_PACKING_INDICES_1S_0P
				inputPackingType='_1S'
				
			elsif (
				(packingIndices==COINSPARK_PACKING_INDICES_ALL_0P) ||
				(packingIndices==COINSPARK_PACKING_INDICES_ALL_1S) ||
				(packingIndices==COINSPARK_PACKING_INDICES_ALL_ALL)
			)
				inputPackingType='_ALL'
			end
			
			# output packing
			
			if (
				(packingIndices==COINSPARK_PACKING_INDICES_0P_0P) ||
				(packingIndices==COINSPARK_PACKING_INDICES_1S_0P) ||
				(packingIndices==COINSPARK_PACKING_INDICES_ALL_0P)
			)
				outputPackingType='_0P'
				
			elsif (
				(packingIndices==COINSPARK_PACKING_INDICES_0P_1S) ||
				(packingIndices==COINSPARK_PACKING_INDICES_ALL_1S)
			)
				outputPackingType='_1S'
				
			elsif (
				(packingIndices==COINSPARK_PACKING_INDICES_0P_ALL) ||
				(packingIndices==COINSPARK_PACKING_INDICES_ALL_ALL)
			)
				outputPackingType='_ALL'
			end
		end

		# Final stage of packing for input && output indices

		@inputs=packingTypeToValues(inputPackingType, previousTransfer ? previousTransfer.inputs : nil, countInputs)
		@outputs=packingTypeToValues(outputPackingType, previousTransfer ? previousTransfer.outputs : nil, countOutputs)

		# Read in the fields as appropriate

		counts=packingToByteCounts(packing, packingExtend)
		
		txIDPrefixBytes=counts['txIDPrefixBytes']
		
		metadataArray=metadata.chars.to_a # split into array of characters for next bit (.to_a for Ruby 1.9)
		
		read_array=[
			shiftReadUnsignedField(metadataArray, counts['blockNumBytes'], @assetRef, '@blockNum'),
			shiftReadUnsignedField(metadataArray, counts['txOffsetBytes'], @assetRef, '@txOffset')
		]
		
		if txIDPrefixBytes==0
			read_array.push(true)
		else
			@assetRef.txIDPrefix=metadataArray[0...txIDPrefixBytes].join('').unpack('H*')[0].upcase
			metadataArray=metadataArray[txIDPrefixBytes..-1]
			read_array.push(@assetRef.txIDPrefix.length==2*txIDPrefixBytes)
		end
				
		read_array+=[
			shiftReadUnsignedField(metadataArray, counts['firstInputBytes'], @inputs, '@first'),
			shiftReadUnsignedField(metadataArray, counts['countInputsBytes'], @inputs, '@count'),
			shiftReadUnsignedField(metadataArray, counts['firstOutputBytes'], @outputs, '@first'),
			shiftReadUnsignedField(metadataArray, counts['countOutputsBytes'], @outputs, '@count'),
			shiftReadUnsignedField(metadataArray, counts['quantityBytes'], self, '@qtyPerOutput')
		]
		
		metadata=metadataArray.join('') # convert any remaining characters back into the string
		
		for read in read_array
			return 0 if !read
		end

		# Finish up reading in quantity
		
		packingQuantity=packing & COINSPARK_PACKING_QUANTITY_MASK
		
		if packingQuantity==COINSPARK_PACKING_QUANTITY_1P
			if previousTransfer
				@qtyPerOutput=previousTransfer.qtyPerOutput
			else
				@qtyPerOutput=1
			end
		
		elsif packingQuantity==COINSPARK_PACKING_QUANTITY_MAX
			@qtyPerOutput=COINSPARK_ASSET_QTY_MAX
			
		elsif packingQuantity==COINSPARK_PACKING_QUANTITY_FLOAT
			decodeQuantity=@qtyPerOutput&COINSPARK_TRANSFER_QTY_FLOAT_MASK
			@qtyPerOutput=mantissaExponentToQty(decodeQuantity%COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE,
				decodeQuantity/COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE)
		end

		# Return bytes used
		
		return 0 if !isValid
		
		return startLength-metadata.length
	end


	def toStringInner(headers)
		buffer=headers ? "COINSPARK TRANSFER\n" : ""
		isDefaultRoute=(@assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
	
		if isDefaultRoute
			buffer+="      Default route:\n"
		else
			buffer+=@assetRef.toStringInner(false)
			buffer+="    Asset reference: "+@assetRef.encode+"\n"
		end

		if @inputs.count>0
			if @inputs.count>1
				buffer+=sprintf("             Inputs: %d - %d (count %d)", @inputs.first, @inputs.first+@inputs.count-1, @inputs.count)
			else
				buffer+=sprintf("              Input: %d", @inputs.first)
			end
		else
			buffer+="             Inputs: none"
		end
		
		buffer+=(" (small endian hex: first "+unsignedToSmallEndianHex(@inputs.first, 2)+" count "+
			unsignedToSmallEndianHex(@inputs.count, 2)+")\n")

		if @outputs.count>0
			if (@outputs.count>1) && !isDefaultRoute
				buffer+=sprintf("            Outputs: %d - %d (count %d)", @outputs.first, @outputs.first+@outputs.count-1, @outputs.count)
			else
				buffer+=sprintf("             Output: %d", @outputs.first)
			end
		else
			buffer+="            Outputs: none"
		end
	
		buffer+=(" (small endian hex: first "+unsignedToSmallEndianHex(@outputs.first, 2)+
			" count "+unsignedToSmallEndianHex(@outputs.count, 2)+")\n")
	
		if !isDefaultRoute
			buffer+="     Qty per output: "+@qtyPerOutput.to_s+" (small endian hex "+unsignedToSmallEndianHex(@qtyPerOutput, 8)
		
			result=qtyToMantissaExponent(@qtyPerOutput, 0,
				COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX, COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX)
		
			if result['qty']==@qtyPerOutput
				encodeQuantity=(result['exponent']*COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE+result['mantissa'])&COINSPARK_TRANSFER_QTY_FLOAT_MASK		
				buffer+=", as float "+unsignedToSmallEndianHex(encodeQuantity, COINSPARK_TRANSFER_QTY_FLOAT_LENGTH)
			end

			buffer+=")\n"
		end
	
		buffer+="END COINSPARK TRANSFER\n\n" if headers
	
		return buffer
	end

	def packingToByteCounts(packing, packingExtend)

		# Set default values for bytes for all fields to zero

		counts={
			'blockNumBytes' => 0,
			'txOffsetBytes' => 0,
			'txIDPrefixBytes' => 0,
		
			'firstInputBytes' => 0,
			'countInputsBytes' => 0,
			'firstOutputBytes' => 0,
			'countOutputsBytes' => 0,
		
			'quantityBytes' => 0
		}
	
		# Packing for genesis reference
		
		packingGenesis=packing & COINSPARK_PACKING_GENESIS_MASK

		if packingGenesis==COINSPARK_PACKING_GENESIS_3_3_BYTES
			counts['blockNumBytes']=3
			counts['txOffsetBytes']=3
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN

		elsif packingGenesis==COINSPARK_PACKING_GENESIS_3_4_BYTES
			counts['blockNumBytes']=3
			counts['txOffsetBytes']=4
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN

		elsif packingGenesis==COINSPARK_PACKING_GENESIS_4_4_BYTES
			counts['blockNumBytes']=4
			counts['txOffsetBytes']=4
			counts['txIDPrefixBytes']=COINSPARK_ASSETREF_TXID_PREFIX_LEN
		end

		# Packing for input and output indices (relevant for extended indices only)

		if (packing & COINSPARK_PACKING_INDICES_MASK) == COINSPARK_PACKING_INDICES_EXTEND
			getCounts=packingExtendAddByteCounts((packingExtend >> COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) &
				COINSPARK_PACKING_EXTEND_MASK, counts['firstInputBytes'], counts['countInputsBytes'], false)
				
			counts['firstInputBytes']=getCounts['firstBytes']
			counts['countInputsBytes']=getCounts['countBytes']
			
			getCounts=packingExtendAddByteCounts((packingExtend >> COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) &
				COINSPARK_PACKING_EXTEND_MASK, counts['firstOutputBytes'], counts['countOutputsBytes'], false)
				
			counts['firstOutputBytes']=getCounts['firstBytes']
			counts['countOutputsBytes']=getCounts['countBytes']
		end

		# Packing for quantity
		
		packingQuantity=packing & COINSPARK_PACKING_QUANTITY_MASK

		if packingQuantity==COINSPARK_PACKING_QUANTITY_1_BYTE
			counts['quantityBytes']=1

		elsif packingQuantity==COINSPARK_PACKING_QUANTITY_2_BYTES
			counts['quantityBytes']=2

		elsif packingQuantity==COINSPARK_PACKING_QUANTITY_3_BYTES
			counts['quantityBytes']=3

		elsif packingQuantity==COINSPARK_PACKING_QUANTITY_4_BYTES
			counts['quantityBytes']=4

		elsif packingQuantity==COINSPARK_PACKING_QUANTITY_6_BYTES
			counts['quantityBytes']=6

		elsif packingQuantity==COINSPARK_PACKING_QUANTITY_FLOAT
			counts['quantityBytes']=COINSPARK_TRANSFER_QTY_FLOAT_LENGTH
		end

		# Return the resulting array
	
		return counts
	end

end


# CoinSparkTransferList class for managing list of asset transfer metadata

class CoinSparkTransferList < CoinSparkBase
	attr_accessor :transfers
	
	def initialize
		clear
	end
	
	def clear
		@transfers=[]
	end

	def toString
		buffer="COINSPARK TRANSFERS\n"
		
		@transfers.each_with_index { |transfer, transferIndex|
			buffer+="\n" if transferIndex>0
			buffer+=transfer.toStringInner(false)
		}

		buffer+="END COINSPARK TRANSFERS\n\n"

		return buffer
	end
		
	def isValid
		return false if !@transfers.is_a? Enumerable
			
		for transfer in @transfers
			return false if !transfer.isValid
		end
				
		return true
	end

	def match(otherTransfers, strict)
		countTransfers=@transfers.length
		return false if countTransfers!=otherTransfers.transfers.length
	
		if strict
			for transferIndex in 0...countTransfers
				return false if !@transfers[transferIndex].match(otherTransfers.transfers[transferIndex])
			end	
	
		else
			thisOrdering=groupOrdering
			otherOrdering=otherTransfers.groupOrdering
		
			for transferIndex in 0...countTransfers
				return false if !@transfers[thisOrdering[transferIndex]].match(otherTransfers.transfers[otherOrdering[transferIndex]])
			end
		end

		return true
	end

	def encode(countInputs, countOutputs, metadataMaxLen)

		# 4-character identifier

		metadata=COINSPARK_METADATA_IDENTIFIER+COINSPARK_TRANSFERS_PREFIX

		# Encode each transfer, grouping by asset reference, but preserving original order otherwise

		ordering=groupOrdering
	
		countTransfers=@transfers.length
		previousTransfer=nil
	
		for transferIndex in 0...countTransfers
			thisTransfer=@transfers[ordering[transferIndex]]
		
			written=thisTransfer.encode(previousTransfer, metadataMaxLen-metadata.length, countInputs, countOutputs)
			return nil if written==nil
			
			metadata+=written
			previousTransfer=thisTransfer
		end

		# Extra length check (even though thisTransfer.encode should be sufficient)

		return nil if metadata.length>metadataMaxLen
		
		# Return what we created

		return metadata
	end

	def decode(metadata, countInputs, countOutputs)
		metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_TRANSFERS_PREFIX)
		return 0 if metadata==nil

		# Iterate over list

		@transfers=[]
		previousTransfer=nil

		while metadata.length>0
			transfer=CoinSparkTransfer.new
			transferBytesUsed=transfer.decode(metadata, previousTransfer, countInputs, countOutputs)
		
			if transferBytesUsed>0
				@transfers.push(transfer)
				metadata=metadata[transferBytesUsed..-1]
				previousTransfer=transfer
		
			else
				return 0 # something was invalid
			end
		end

		# Return count

		return @transfers.length
	end

	def calcMinFee(countInputs, outputsSatoshis, outputsRegular)
		countOutputs=outputsSatoshis.length
		return COINSPARK_SATOSHI_QTY_MAX if countOutputs!=outputsRegular.length # these two arrays must be the same size
	
		transfersToCover=0
	
		for transfer in @transfers
			if (
				(transfer.assetRef.blockNum != COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) && # don't count default routes
				(transfer.inputs.count>0) &&
				(transfer.inputs.first<countInputs) # only count if at least one valid input index
			)
				outputIndex=[transfer.outputs.first, 0].max
				lastOutputIndex=[transfer.outputs.first+transfer.outputs.count, countOutputs].min-1
				
				while outputIndex<=lastOutputIndex
					transfersToCover+=1 if outputsRegular[outputIndex]
					outputIndex+=1
				end
			end
		end
	
		return transfersToCover*getMinFeeBasis(outputsSatoshis, outputsRegular)
	end

	def apply(assetRef, genesis, _inputBalances, outputsRegular)
		inputBalances=_inputBalances.clone.map(&:to_i) # copy since we will modify it, && cast to integers

		# Zero output quantities and get counts
	
		countInputs=inputBalances.length
		countOutputs=outputsRegular.length
		outputBalances=[0]*countOutputs
	
		# Perform explicit transfers (i.e. not default routes)
		
		for transfer in @transfers
			if assetRef.match(transfer.assetRef)
				inputIndex=[transfer.inputs.first, 0].max
				outputIndex=[transfer.outputs.first, 0].max

				lastInputIndex=[inputIndex+transfer.inputs.count, countInputs].min-1
				lastOutputIndex=[outputIndex+transfer.outputs.count, countOutputs].min-1
	
				while outputIndex<=lastOutputIndex
					if outputsRegular[outputIndex]
						transferRemaining=transfer.qtyPerOutput
			
						while inputIndex<=lastInputIndex
							transferQuantity=[transferRemaining, inputBalances[inputIndex]].min
				
							if transferQuantity>0 # skip all this if nothing is to be transferred (branch not really necessary)
								inputBalances[inputIndex]-=transferQuantity
								transferRemaining-=transferQuantity
								outputBalances[outputIndex]+=transferQuantity
							end
				
							if transferRemaining>0
								inputIndex+=1 # move to next input since self one is drained
							else
								break # stop if we have nothing left to transfer
							end
						end
					end

					outputIndex+=1
				end
			end
		end
	
		# Apply payment charges to all quantities not routed by default

		for outputIndex in 0...countOutputs
			if outputsRegular[outputIndex]
				outputBalances[outputIndex]=genesis.calcNet(outputBalances[outputIndex])
			end
		end
			
		# Send remaining quantities to default outputs

		inputDefaultOutput=getDefaultRouteMap(countInputs, outputsRegular)
		
		for inputIndex in 0...inputDefaultOutput.length
			outputIndex=inputDefaultOutput[inputIndex]
	
			outputBalances[outputIndex]+=inputBalances[inputIndex] if outputIndex!=nil
		end
			
		# Return the result
		return outputBalances
	end

	def applyNone(assetRef, genesis, inputBalances, outputsRegular)
		countOutputs=outputsRegular.length
		outputBalances=[0]*countOutputs

		outputIndex=getLastRegularOutput(outputsRegular)
		if outputIndex!=nil
			for inputBalance in inputBalances
				outputBalances[outputIndex]+=inputBalance.to_i # to prevent concatenation
			end
		end
		
		return outputBalances
	end
	
	def defaultOutputs(countInputs, outputsRegular)
		outputsDefault=[false]*outputsRegular.length
	
		inputDefaultOutput=getDefaultRouteMap(countInputs, outputsRegular)
		
		for outputIndex in inputDefaultOutput
			outputsDefault[outputIndex]=true if outputIndex!=nil
		end
			
		return outputsDefault
	end

	def groupOrdering
		countTransfers=@transfers.length
		transferUsed=[false]*countTransfers
		ordering=[nil]*countTransfers

		for orderIndex in 0...countTransfers
			bestTransferScore=0
			bestTransferIndex=-1
		
			for transferIndex in 0...countTransfers
				transfer=@transfers[transferIndex]
			
				if !transferUsed[transferIndex]
					if transfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE
						transferScore=3 # top priority to default routes, which must be first in the encoded list
					elsif (orderIndex>0) && transfer.assetRef.match(@transfers[ordering[orderIndex-1]].assetRef)
						transferScore=2 # then next best is one which has same asset reference as previous
					else
						transferScore=1 # otherwise any will do
					end
					
					if transferScore>bestTransferScore # if it's clearly the best, take it
						bestTransferScore=transferScore
						bestTransferIndex=transferIndex
					
					elsif transferScore==bestTransferScore # otherwise give priority to "lower" asset references
						if transfer.assetRef.compare(@transfers[bestTransferIndex].assetRef)<0
							bestTransferIndex=transferIndex
						end
					end
				end
			end
			
			ordering[orderIndex]=bestTransferIndex
			transferUsed[bestTransferIndex]=true
		end
	
		return ordering
	end

	def getDefaultRouteMap(countInputs, outputsRegular)
		countOutputs=outputsRegular.length
	
		# Default to last output for all inputs

		inputDefaultOutput=[getLastRegularOutput(outputsRegular)]*countInputs
	
		# Apply any default route transfers in reverse order (since early ones take precedence)
		
		for transfer in @transfers.reverse
			if transfer.assetRef.blockNum==COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE
				outputIndex=transfer.outputs.first
			
				if (outputIndex>=0) && (outputIndex<countOutputs)
					inputIndex=[transfer.inputs.first, 0].max
					lastInputIndex=[inputIndex+transfer.inputs.count, countInputs].min-1
					
					while inputIndex<=lastInputIndex
						inputDefaultOutput[inputIndex]=outputIndex
						inputIndex+=1
					end
				end
			end
		end
		
		# Return the result
		
		return inputDefaultOutput
	end
	
end


# CoinSparkPaymentRef class for managing payment references

class CoinSparkPaymentRef < CoinSparkBase
	attr_accessor :ref

	def initialize
		clear
	end

	def clear
		@ref=0
	end
	
	def toString
		buffer="COINSPARK PAYMENT REFERENCE\n"
		buffer+=@ref.to_s+" (small endian hex "+unsignedToSmallEndianHex(@ref, 8)+")\n"
		buffer+="END COINSPARK PAYMENT REFERENCE\n\n"
		
		return buffer
	end

	def isValid
		return isInteger(@ref) && (@ref>=0) && (@ref<=COINSPARK_PAYMENT_REF_MAX)
	end

	def match(otherPaymentRef)
		return @ref==otherPaymentRef.ref
	end
	
	def randomize
		@ref=0
		
		bitsRemaining=COINSPARK_PAYMENT_REF_MAX
		while bitsRemaining>0
			@ref*=8192
			@ref+=rand(8192)
			bitsRemaining=(bitsRemaining/8192).to_i
		end
	
		@ref=@ref%(1+COINSPARK_PAYMENT_REF_MAX)
		
		return @ref
	end

	def encode(metadataMaxLen)
		return nil if !isValid
		
		# 4-character identifier
		
		metadata=COINSPARK_METADATA_IDENTIFIER+COINSPARK_PAYMENTREF_PREFIX

		# The payment reference
	
		bytes=0
		paymentLeft=@ref
		while paymentLeft>0
			bytes+=1
			paymentLeft=(paymentLeft/256).to_i
		end
		
		metadata=metadata+writeSmallEndianUnsigned(@ref, bytes)
	
		# Check the total length is within the specified limit

		return nil if metadata.length>metadataMaxLen
		
		# Return what we created

		return metadata
	end

	def decode(metadata)
		metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_PAYMENTREF_PREFIX)
		return nil if metadata==nil
		
		# The payment reference

		finalMetadataLen=metadata.length
		return nil if finalMetadataLen>8

		@ref=readSmallEndianUnsigned(metadata, finalMetadataLen)
		
		# Return validity

		return isValid	
	end

end


# CoinSparkMessage class for managing message metadata

class CoinSparkMessage < CoinSparkBase
	attr_accessor :useHttps
	attr_accessor :serverHost
	attr_accessor :usePrefix
	attr_accessor :serverPath
	attr_accessor :isPublic
	attr_accessor :outputRanges
	attr_accessor :hash
	attr_accessor :hashLen
		
	COINSPARK_OUTPUTS_MORE_FLAG=0x80
	COINSPARK_OUTPUTS_RESERVED_MASK=0x60
	COINSPARK_OUTPUTS_TYPE_MASK=0x18
	COINSPARK_OUTPUTS_TYPE_SINGLE=0x00 # one output index (0...7)
	COINSPARK_OUTPUTS_TYPE_FIRST=0x08 # first (0...7) outputs
	COINSPARK_OUTPUTS_TYPE_UNUSED=0x10 # for future use
	COINSPARK_OUTPUTS_TYPE_EXTEND=0x18 # "extend", including public/all
	COINSPARK_OUTPUTS_VALUE_MASK=0x07
	COINSPARK_OUTPUTS_VALUE_MAX=7
	
	def initialize
		clear
	end

	def clear
		@useHttps=false
		@serverHost=''
		@usePrefix=false
		@serverPath=''
		@isPublic=false
		@outputRanges=[]
		@hash=''
		@hashLen=0
	end
	
	def toString	
		hostPathMetadata=encodeDomainAndOrPath(@serverHost, @useHttps, @serverPath, @usePrefix, true)
		urlString=calcServerURL
		
		buffer="COINSPARK MESSAGE\n"
		buffer+=sprintf("    Server URL: %s (length %d+%d encoded %s length %d)\n", urlString,
			@serverHost.length, @serverPath.length, hostPathMetadata.unpack('H*')[0].upcase, hostPathMetadata.length)
		buffer+=sprintf("Public message: %s\n", @isPublic ? "yes" : "no")
		
		for outputRange in @outputRanges
			if outputRange.count>0
				if outputRange.count>1
					buffer+=sprintf("       Outputs: %d - %d (count %d)", outputRange.first, outputRange.first+outputRange.count-1, outputRange.count)
				else
					buffer+=sprintf("        Output: %d", outputRange.first)
				end
			else
				buffer+="       Outputs: none"
			end
	
			buffer+=sprintf(" (small endian hex: first %s count %s)\n", unsignedToSmallEndianHex(outputRange.first, 2),
				unsignedToSmallEndianHex(outputRange.count, 2))
		end
		
		buffer+=sprintf("  Message hash: %s (length %d)\n", @hash[0...hashLen].unpack('H*')[0].upcase, @hashLen)
		buffer+="END COINSPARK MESSAGE\n\n"
		
		return buffer
	end
	
	def isValid
		return false if !(
			isBoolean(@useHttps) &&
			isString(@serverHost) &&
			isBoolean(@usePrefix) &&
			isString(@serverPath) &&
			isBoolean(@isPublic) &&
			(@outputRanges.is_a? Enumerable) &&
			isString(@hash) &&
			isInteger(@hashLen)
		)
		
		return false if @serverHost.length>COINSPARK_MESSAGE_SERVER_HOST_MAX_LEN

		return false if @serverPath.length>COINSPARK_MESSAGE_SERVER_PATH_MAX_LEN
			
		return false if @hash.length<@hashLen # check we have at least as much data as specified by @hashLen

		return false if (@hashLen<COINSPARK_MESSAGE_HASH_MIN_LEN) || (@hashLen>COINSPARK_MESSAGE_HASH_MAX_LEN)

		return false if (!@isPublic) && (@outputRanges.length==0) # public or aimed at some outputs at least

		return false if @outputRanges.length>COINSPARK_MESSAGE_MAX_IO_RANGES

		for outputRange in @outputRanges
			return false if !outputRange.isValid
		end

		return true
	end
	
	def match(otherMessage, strict)	
		hashCompareLen=[@hashLen, otherMessage.hashLen, COINSPARK_MESSAGE_HASH_MAX_LEN].min

		if strict
			thisRanges=@outputRanges
			otherRanges=otherMessage.outputRanges
		else
			thisRanges=normalizeIORanges(@outputRanges)
			otherRanges=normalizeIORanges(otherMessage.outputRanges)
		end
		
		return false if thisRanges.length != otherRanges.length
			
		for index in 0...thisRanges.length
			return false if !thisRanges[index].match(otherRanges[index])
		end
		
		return (
			(@useHttps==otherMessage.useHttps) &&
			(@serverHost.downcase==otherMessage.serverHost.downcase) &&
			(@usePrefix==otherMessage.usePrefix) &&
			(@serverPath.downcase==otherMessage.serverPath.downcase) &&
			(@isPublic==otherMessage.isPublic) &&
			(@hash[0...hashCompareLen]==otherMessage.hash[0...hashCompareLen])
		)
	end	
	
	def encode(countOutputs, metadataMaxLen)
		return nil if !isValid

		# 4-character identifier
	
		metadata=COINSPARK_METADATA_IDENTIFIER+COINSPARK_MESSAGE_PREFIX

		# Server host and path
		
		written=encodeDomainAndOrPath(@serverHost, @useHttps, @serverPath, @usePrefix, true)
		return nil if written==nil

		metadata+=written

		# Output ranges

		if @isPublic # add public indicator first
			packing=(((@outputRanges.length>0) ? COINSPARK_OUTPUTS_MORE_FLAG : 0) |
				COINSPARK_OUTPUTS_TYPE_EXTEND | COINSPARK_PACKING_EXTEND_PUBLIC)
			metadata+=packing.chr
		end
		
		for index in 0...@outputRanges.length # other output ranges
			outputRange=@outputRanges[index]
			
			packingResult=getOutputRangePacking(outputRange, countOutputs)
			return nil if packingResult==nil
	
			# The packing byte
			
			packing=packingResult['packing']
			packing|=COINSPARK_OUTPUTS_MORE_FLAG if (index+1)<@outputRanges.length

			metadata+=packing.chr
	
			# The index of the first output, if necessary
	
			written=writeUnsignedField(packingResult['firstBytes'], outputRange.first)
			return nil if written==nil
			
			metadata+=written
	
			# The number of outputs, if necessary
			
			written=writeUnsignedField(packingResult['countBytes'], outputRange.count)
			return nil if written==nil
			
			metadata+=written
		end

		# Message hash
	
		metadata+=@hash[0...@hashLen]

		# Check the total length is within the specified limit
	
		return nil if metadata.length>metadataMaxLen
			
		# Return what we created
	
		return metadata
	end
	
	def decode(metadata, countOutputs)
		metadata=CoinSparkLocateMetadataRange(metadata, COINSPARK_MESSAGE_PREFIX)
		return false if metadata==nil
			
		# Server host and path
		
		decodedHostPath=decodeDomainAndOrPath(metadata, true, true, true)
		return false if decodedHostPath==nil
		
		metadata=metadata[decodedHostPath['decodedChars']..-1]
		@useHttps=decodedHostPath['useHttps']
		@serverHost=decodedHostPath['domainName']
		@usePrefix=decodedHostPath['usePrefix']
		@serverPath=decodedHostPath['pagePath']

		# Output ranges

		@isPublic=false
		@outputRanges=[]
		readAnotherRange=true # since Ruby has no do...while construct

		while readAnotherRange
			packing=readSmallEndianUnsigned(metadata, 1) # Read the next packing byte and check reserved bits are zero
			metadata=metadata[1..-1]
			return false if packing==nil
 
			return false if (packing & COINSPARK_OUTPUTS_RESERVED_MASK)!=0
	
			readAnotherRange=(packing & COINSPARK_OUTPUTS_MORE_FLAG)!=0
			packingType=packing & COINSPARK_OUTPUTS_TYPE_MASK
			packingValue=packing & COINSPARK_OUTPUTS_VALUE_MASK
	
			if (packingType==COINSPARK_OUTPUTS_TYPE_EXTEND) && (packingValue==COINSPARK_PACKING_EXTEND_PUBLIC)
				@isPublic=true # special case for public messages
	
			else # Create a new output range		
				return false if @outputRanges.length>=COINSPARK_MESSAGE_MAX_IO_RANGES # too many output ranges
					
				firstBytes=0
				countBytes=0
		
				# Decode packing byte
		
				if packingType==COINSPARK_OUTPUTS_TYPE_SINGLE # inline single input
					outputRange=CoinSparkIORange.new
					outputRange.first=packingValue
					outputRange.count=1
	   
				elsif packingType==COINSPARK_OUTPUTS_TYPE_FIRST # inline first few outputs
					outputRange=CoinSparkIORange.new
					outputRange.first=0
					outputRange.count=packingValue
			
				elsif packingType==COINSPARK_OUTPUTS_TYPE_EXTEND # we'll be taking additional bytes
					extendPackingType=decodePackingExtend(packingValue, true)
					return false if extendPackingType==nil
					
					outputRange=packingTypeToValues(extendPackingType, nil, countOutputs)
					
					getCounts=packingExtendAddByteCounts(packingValue, firstBytes, countBytes, true)
					firstBytes=getCounts['firstBytes']
					countBytes=getCounts['countBytes']
			
				else
					return false # will be COINSPARK_OUTPUTS_TYPE_UNUSED
				end
		
				# The index of the first output and number of outputs, if necessary
				
				metadataArray=metadata.chars.to_a # split into array of characters for next bit (.to_a for Ruby 1.9)
				
				return false if !shiftReadUnsignedField(metadataArray, firstBytes, outputRange, '@first')
				return false if !shiftReadUnsignedField(metadataArray, countBytes, outputRange, '@count')
					
				metadata=metadataArray.join('') # convert any remaining characters back into the string
				
				# Add on the new output range
			
				@outputRanges.push(outputRange)
			end
		end
			
		# Message hash
	
		@hashLen=[metadata.length, COINSPARK_MESSAGE_HASH_MAX_LEN].min
		@hash=metadata.slice(0...@hashLen) # insufficient length will be caught by isValid

		# Return validity

		return isValid
	end
	
	def hasOutput(outputIndex)
		for outputRange in @outputRanges
			return true if (outputIndex>=outputRange.first) && (outputIndex<(outputRange.first+outputRange.count))
		end	

		return false
	end
	
	def calcHashLen(countOutputs, metadataMaxLen)
		hashLen=metadataMaxLen-COINSPARK_METADATA_IDENTIFIER_LEN-1

		hostPathLen=@serverPath.length+1

		if readIPv4Address(@serverHost)
			hashLen-=5 # packing and IP octets
			hostPathLen=0 if hostPathLen==1 # will skip server path in this case
		else
			hashLen-=1 # packing
			hostPathLen+=shrinkLowerDomainName(@serverHost)['domainName'].length+1
		end
		
		hashLen-=2*(((hostPathLen+2)/3).to_i) # uses integer arithmetic

		hashLen-=1 if @isPublic
		
		for outputRange in @outputRanges
			packingResult=getOutputRangePacking(outputRange, countOutputs)
			if packingResult!=nil
				hashLen-=(1+packingResult['firstBytes']+packingResult['countBytes'])
			end
		end

		return [[hashLen, 0].max, COINSPARK_MESSAGE_HASH_MAX_LEN].min
	end
	
	def calcServerURL
		return (
			(@useHttps ? 'https' : 'http')+
			'://'+@serverHost+'/'+
			(@usePrefix ? 'coinspark/' : '')+
			@serverPath+
			((@serverPath.length>0) ? '/' : '')
		).downcase
	end	
	
	def getOutputRangePacking(outputRange, countOutputs)
		packingOptions=getPackingOptions(nil, outputRange, countOutputs, true)

		firstBytes=0
		countBytes=0

		if packingOptions['_1_0_BYTE'] && (outputRange.first<=COINSPARK_OUTPUTS_VALUE_MAX) # inline single output
			packing=COINSPARK_OUTPUTS_TYPE_SINGLE | (outputRange.first & COINSPARK_OUTPUTS_VALUE_MASK)

		elsif packingOptions['_0_1_BYTE'] && (outputRange.count<=COINSPARK_OUTPUTS_VALUE_MAX) # inline first few outputs
			packing=COINSPARK_OUTPUTS_TYPE_FIRST | (outputRange.count & COINSPARK_OUTPUTS_VALUE_MASK)

		else # we'll be taking additional bytes
			packingExtend=encodePackingExtend(packingOptions)
			return nil if packingExtend==nil
	
			packingResult=packingExtendAddByteCounts(packingExtend, firstBytes, countBytes, true)
			firstBytes=packingResult['firstBytes']
			countBytes=packingResult['countBytes']
	
			packing=COINSPARK_OUTPUTS_TYPE_EXTEND | (packingExtend & COINSPARK_OUTPUTS_VALUE_MASK)
		end

		return {'packing' => packing, 'firstBytes' => firstBytes, 'countBytes' => countBytes}
	end
end


# Class used internally for input || output ranges

class CoinSparkIORange < CoinSparkBase
	attr_accessor :first
	attr_accessor :count
	
	def initialize
		clear
	end

	def clear
		@first=0
		@count=0
	end

	def isValid
		return false if (@first<0) || (@first>COINSPARK_IO_INDEX_MAX) || !isInteger(@first)
		
		return false if (@count<0) || (@count>COINSPARK_IO_INDEX_MAX) || !isInteger(@count)
			
		return true
	end

	def match(otherInOutRange)
		return (@first==otherInOutRange.first) && (@count==otherInOutRange.count)
	end
end


# Other functions used internally

def CoinSparkGetRawScript(scriptPubKey, scriptIsHex)
	if scriptIsHex
		return nil if ((scriptPubKey.length%2)!=0) || scriptPubKey[/\H/] # check valid hex
		return [scriptPubKey].pack('H*')
	end
	
	return scriptPubKey.clone.force_encoding('BINARY')
end


def CoinSparkLocateMetadataRange(metadata, desiredPrefix)
	metadataLen=metadata.length

	return nil if metadataLen<(COINSPARK_METADATA_IDENTIFIER_LEN+1) # check for 4 bytes at least
	
	return nil if metadata[0...COINSPARK_METADATA_IDENTIFIER_LEN]!=COINSPARK_METADATA_IDENTIFIER # check it starts 'SPK'
		 
	position=COINSPARK_METADATA_IDENTIFIER_LEN # start after 'SPK'

	while position<metadataLen
		foundPrefix=metadata[position] # read the next prefix
		position+=1
		foundPrefixOrd=foundPrefix.ord
		
		return metadata[position..-1] if desiredPrefix ? (foundPrefix==desiredPrefix) : (foundPrefixOrd>COINSPARK_LENGTH_PREFIX_MAX)
			# it's our data from here to the end (if desiredPrefix==nil, it matches the last one whichever it is)
	
		return nil if foundPrefixOrd>COINSPARK_LENGTH_PREFIX_MAX # it's some other type of data from here to end
		
		# if we get here it means we found a length byte
	
		return nil if (position+foundPrefixOrd)>metadataLen # something went wrong - length indicated is longer than that available
		
		return nil if position>=metadataLen # something went wrong - that was the end of the input data
		
		if metadata[position]==desiredPrefix # it's the length of our part
			return metadata[position+1...position+foundPrefixOrd]
		else
			position+=foundPrefixOrd # skip over this many bytes
		end
	end

	return nil
end