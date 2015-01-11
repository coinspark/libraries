import sys, os.path, random
from coinspark import *


def GetNextInputLine(inputSource):
	inputLine=inputSource.readline()
	if len(inputLine)==0: # empty string indicates end of file
		return None;
	
	inputLine=inputLine.rstrip("\n")
	
	hashPos=inputLine.find(' # ')
	if hashPos>=0:
		inputLine=inputLine[:hashPos]
	
	return inputLine
	
	
def GetNextInputLines(inputSource, countLines):
	inputLines=[]
	
	while (countLines>0):
		countLines-=1
		
		inputLine=GetNextInputLine(inputSource)
		if inputLine is None:
			return None
			
		inputLines.append(inputLine)
		
	return inputLines


def ProcessInputContents(inputSource):
	header=GetNextInputLine(inputSource)
	GetNextInputLine(inputSource) # discard blank line
	
	if header=='CoinSpark Address Tests Input':
		ProcessAddressTests(inputSource)
		
	elif header=='CoinSpark AssetRef Tests Input':
		ProcessAssetRefTests(inputSource)
		
	elif header=='CoinSpark Script Tests Input':
		ProcessScriptTests(inputSource)
		
	elif header=='CoinSpark AssetHash Tests Input':
		ProcessAssetHashTests(inputSource)
		
	elif header=='CoinSpark Genesis Tests Input':
		ProcessGenesisTests(inputSource)
		
	elif header=='CoinSpark Transfer Tests Input':
		ProcessTransferTests(inputSource)
		
	elif header=='CoinSpark MessageHash Tests Input':
		ProcessMessageHashTests(inputSource)
		
		
def ProcessAddressTests(inputSource):
	print("CoinSpark Address Tests Output\n")
	
	while True:
		inputLine=GetNextInputLine(inputSource)
		if inputLine is None:
			break
			
		address=CoinSparkAddress()
		
		if address.decode(inputLine):
			sys.stdout.write(address.toString())
		else:
			sys.exit("Failed to decode address: "+inputLine)
			
		encoded=address.encode()
		
		if encoded!=inputLine:
			sys.exit("Encode address mismatch: "+encoded+" should be "+inputLine)
			
		if not address.match(address):
			sys.exit("Failed to match address to itself!")
			

def ProcessAssetRefTests(inputSource):
	print("CoinSpark AssetRef Tests Output\n")
	
	while True:
		inputLine=GetNextInputLine(inputSource)
		if inputLine is None:
			break
	
		assetRef=CoinSparkAssetRef()
	
		if assetRef.decode(inputLine):
			sys.stdout.write(assetRef.toString())
		else:
			sys.exit("Failed to decode AssetRef: "+inputLine)
		
		encoded=assetRef.encode()
	
		if encoded!=inputLine:
			sys.exit("Encode AssetRef mismatch: "+encoded+" should be "+inputLine)
	
		if not assetRef.match(assetRef):
			sys.exit("Failed to match assetRef to itself!")


def ProcessScriptTests(inputSource):
	print("CoinSpark Script Tests Output\n");
	
	while True:
		inputLines=GetNextInputLines(inputSource, 4)
		if inputLines is None:
			break
			
		countInputs, countOutputs, scriptPubKeyHex, dummy = inputLines
		
		metadata=CoinSparkScriptToMetadata(scriptPubKeyHex, True)
		if metadata is None:
			sys.exit("Could not decode script metadata: "+scriptPubKeyHex);
			
		# Read in the different types of metadata
		
		genesis=CoinSparkGenesis()
		hasGenesis=genesis.decode(metadata)
		
		paymentRef=CoinSparkPaymentRef()
		hasPaymentRef=paymentRef.decode(metadata)
		
		transfers=CoinSparkTransferList()
		hasTransfers=transfers.decode(metadata, countInputs, countOutputs)
		
		message=CoinSparkMessage()
		hasMessage=message.decode(metadata, countOutputs)
		
		# Output the toString()s
		
		if hasGenesis:
			sys.stdout.write(genesis.toString())
			
		if hasPaymentRef:
			sys.stdout.write(paymentRef.toString())
			
		if hasTransfers:
			sys.stdout.write(transfers.toString())
			
		if hasMessage:
			sys.stdout.write(message.toString())
			
		# Re-encode
		
		testMetadata=''
		testMetadataMaxLen=len(metadata)
		nextMetadataMaxLen=testMetadataMaxLen
		
		encodeOrder=['genesis', 'paymentRef', 'transfers', 'message']
		
		for encodeField in encodeOrder:
			triedNextMetadata=False
			
			if encodeField=='genesis':
				if hasGenesis:
					nextMetadata=genesis.encode(nextMetadataMaxLen)
					triedNextMetadata=True
			
			elif encodeField=='paymentRef':
				if hasPaymentRef:
					nextMetadata=paymentRef.encode(nextMetadataMaxLen)
					triedNextMetadata=True
			
			elif encodeField=='transfers':
				if hasTransfers:
					nextMetadata=transfers.encode(countInputs, countOutputs, nextMetadataMaxLen)
					triedNextMetadata=True
					
			elif encodeField=='message':
				if hasMessage:
					nextMetadata=message.encode(countOutputs, nextMetadataMaxLen)
					triedNextMetadata=True
					
			if triedNextMetadata:
				if nextMetadata is None:
					sys.exit("Failed to reencode "+encodeField+" metadata!")
				
				if len(testMetadata):
					testMetadata=CoinSparkMetadataAppend(testMetadata, testMetadataMaxLen, nextMetadata)
					if testMetadata is None:
						sys.exit("Insufficient space to append "+encodeField+" metadata!")
			
				else:
					testMetadata=nextMetadata
				
				nextMetadataMaxLen=CoinSparkMetadataMaxAppendLen(testMetadata, testMetadataMaxLen)
				
		# Test other library functions while we are here
		
		if hasGenesis:
			if not genesis.match(genesis, True):
				sys.exit("Failed to match genesis to itself!")
				
			if genesis.calcHashLen(len(metadata))!=genesis.assetHashLen: # assumes that metadata only contains genesis
				sys.exit("Failed to calculate matching hash length!")
				
			testGenesis=CoinSparkGenesis()
			testGenesis.decode(metadata)
			
			rounding=random.randint(-1, 1)
			
			testGenesis.setQty(0, 0)
			testGenesis.setQty(genesis.getQty(), rounding)
			
			testGenesis.setChargeFlat(0, 0)
			testGenesis.setChargeFlat(genesis.getChargeFlat(), rounding)
			
			if not genesis.match(testGenesis, False):
				sys.exit("Mismatch on genesis rounding!")
				
		if hasPaymentRef:
			if not paymentRef.match(paymentRef):
				sys.exit("Failed to match paymentRef to itself!")
				
		if hasTransfers:
			if not transfers.match(transfers, True):
				sys.exit("Failed to strictly match transfers to itself!")
				
			if not transfers.match(transfers, False):
				sys.exit("Failed to leniently match transfers to itself!")
				
		if hasMessage:
			if not message.match(message, True):
				sys.exit("Failed to strictly match message to itself!")
				
			if not message.match(message, False):
				sys.exit("Failed to leniently match message to itself!")
				
			messageEncode=message.encode(countOutputs, len(metadata)) # encode on its own to check calcHashLen()
			
			if message.calcHashLen(countOutputs, len(messageEncode))!=message.hashLen:
				sys.exit("Failed to calculate matching message hash length!")
				
		# Compare to the original
		
		encoded=CoinSparkMetadataToScript(testMetadata, True)
		
		if encoded!=scriptPubKeyHex:
			sys.exit("Encode metadata mismatch: "+encoded+" should be "+scriptPubKeyHex)
		
		checkMetadata=CoinSparkScriptToMetadata(CoinSparkMetadataToScript(testMetadata, False), False)
		
		if checkMetadata!=testMetadata:
			sys.exit("Binary metadata to/from script mismatch!")
			


def ProcessAssetHashTests(inputSource):
	print("CoinSpark AssetHash Tests Output\n")
	
	while True:
		inputLines=GetNextInputLines(inputSource, 10)
		if inputLines is None:
			break
			
		name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent, dummy = inputLines
		
		contractContent=contractContent.encode('utf-8')
		
		hash=CoinSparkCalcAssetHash(name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent)
		
		print(binascii.hexlify(hash).upper().decode('utf-8'))


def ProcessGenesisTests(inputSource):
	print("CoinSpark Genesis Tests Output\n")
	
	while True:
		inputLines=GetNextInputLines(inputSource, 7)
		if inputLines is None:
			break
	
		# Break apart and decode the input lines
		
		firstSpentTxId, firstSpentVout, metadataHex, outputsSatoshisString, outputsRegularString, feeSatoshis, dummy = inputLines

		genesis=CoinSparkGenesis()
		if not genesis.decode(CoinSparkHexToRawString(metadataHex)):
			sys.exit("Failed to decode genesis metadata: "+metadataHex)
			
		outputsSatoshis=outputsSatoshisString.split(',')
		outputsRegular=[bool(int(outputRegular)) for outputRegular in outputsRegularString.split(',')]		
		countOutputs=len(outputsSatoshis)
	
		validFeeSatoshis=genesis.calcMinFee(outputsSatoshis, outputsRegular)
			
		# Perform the genesis calculation
		
		if float(feeSatoshis)>=validFeeSatoshis:
			outputBalances=genesis.apply(outputsRegular)
		else:
			outputBalances=[0] * countOutputs
			
		# Output the results
		
		print("%.0f # transaction fee satoshis to be valid" % validFeeSatoshis)
		print(",".join(["%.0f" % outputBalance for outputBalance in outputBalances])+" # units of the asset in each output")
		print(genesis.calcAssetURL(firstSpentTxId, firstSpentVout)+" # asset web page URL\n")
		
		
def ProcessTransferTests(inputSource):
	print("CoinSpark Transfer Tests Output\n")
	
	while True:
		inputLines=GetNextInputLines(inputSource, 8)
		if inputLines is None:
			break
	
		# Break apart and decode the input lines
		
		(genesisMetadataHex, assetRefString, transfersMetadataHex, inputBalancesString,
			outputsSatoshisString, outputsRegularString, feeSatoshis, dummy) = inputLines

		genesis=CoinSparkGenesis()
		if not genesis.decode(CoinSparkHexToRawString(genesisMetadataHex)):
			sys.exit("Failed to decode genesis metadata: "+genesisMetadataHex)
			
		assetRef=CoinSparkAssetRef()
		if not assetRef.decode(assetRefString):
			sys.exit("Failed to decode asset reference: assetRefString")
			
		inputBalances=inputBalancesString.split(',')
		outputsSatoshis=outputsSatoshisString.split(',')
		outputsRegular=[bool(int(outputRegular)) for outputRegular in outputsRegularString.split(',')]	
		
		countInputs=len(inputBalances)
		countOutputs=len(outputsSatoshis)
	
		transfers=CoinSparkTransferList()
		if not transfers.decode(CoinSparkHexToRawString(transfersMetadataHex), countInputs, countOutputs):
			sys.exit("Failed to decode transfers metadata: "+transfersMetadataHex)
		validFeeSatoshis=transfers.calcMinFee(countInputs, outputsSatoshis, outputsRegular)
			
		# Perform the transfer calculation and get default flags
		
		if float(feeSatoshis)>=validFeeSatoshis:
			outputBalances=transfers.apply(assetRef, genesis, inputBalances, outputsRegular)
		else:
			outputBalances=transfers.applyNone(assetRef, genesis, inputBalances, outputsRegular)
			
		outputsDefault=transfers.defaultOutputs(countInputs, outputsRegular)
		
		# Output the results
		
		print("%.0f # transaction fee satoshis to be valid" % validFeeSatoshis)
		print(",".join(["%.0f" % outputBalance for outputBalance in outputBalances])+" # units of this asset in each output")
		print(",".join(['1' if outputDefault else '0' for outputDefault in outputsDefault])+" # boolean flags whether each output is in a default route\n")
		
		# Test the net and gross calculations using the input balances as example net values
	
		for inputBalance in inputBalances:
			testGrossBalance=genesis.calcGross(inputBalance)
			testNetBalance=genesis.calcNet(testGrossBalance)
			
			if float(inputBalance)!=testNetBalance:
				sys.exit("Net to gross to net mismatch: %.0f -> %.0f -> %.0f!" % (float(inputBalance), testGrossBalance, testNetBalance))
		

def ProcessMessageHashTests(inputSource):
	print("CoinSpark MessageHash Tests Output\n")
	
	while True:
		inputLines=GetNextInputLines(inputSource, 2)
		if inputLines is None:
			break
			
		salt, countParts = inputLines

		salt=salt.encode('utf-8')
		countParts=int(countParts)
		
		inputLines=GetNextInputLines(inputSource, 3*countParts+1)
		if inputLines is None:
			break
		
		messageParts=[]
		while ((len(messageParts)<countParts) and (len(inputLines)>0)):
			messageParts.append({
				'mimeType': inputLines.pop(0),
				'fileName': inputLines.pop(0),
				'content': inputLines.pop(0).encode('utf-8')
			})
			
		hash=CoinSparkCalcMessageHash(salt, messageParts)
		
		print(binascii.hexlify(hash).upper().decode('utf-8'))


# Main entry point for code
	
if len(sys.argv)<2:
	sys.exit("No CoinSpark test input file was specified\n")

inputFileName=sys.argv[1]

if os.path.isfile(inputFileName):
	inputSource=open(inputFileName, 'r')
	ProcessInputContents(inputSource);
	inputSource.close()

else:
	sys.exit("The CoinSpark test input file was wrong\n")
	
