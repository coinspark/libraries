# CoinSpark 2.1 - Ruby test suite
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


require_relative 'coinspark.rb'


def GetNextInputLine(inputSource)
	inputLine=inputSource.gets
	return nil if inputLine==nil
	
	inputLine=inputLine.gsub(/\n$/, '')
	
	hashPos=inputLine.index(' # ')
	inputLine=inputLine.slice(0, hashPos) if hashPos!=nil
	
	return inputLine
end
	
	
def GetNextInputLines(inputSource, countLines)
	inputLines=[]
	
	while countLines>0
		countLines-=1
		
		inputLine=GetNextInputLine(inputSource)
		return nil if inputLine==nil
			
		inputLines.push(inputLine)
	end
		
	return inputLines
end


def ProcessInputContents(inputSource)
	header=GetNextInputLine(inputSource)
	GetNextInputLine(inputSource) # discard blank line
	
	if header=='CoinSpark Address Tests Input'
		ProcessAddressTests(inputSource)
		
	elsif header=='CoinSpark AssetRef Tests Input'
		ProcessAssetRefTests(inputSource)
		
	elsif header=='CoinSpark Script Tests Input'
		ProcessScriptTests(inputSource)
		
	elsif header=='CoinSpark AssetHash Tests Input'
		ProcessAssetHashTests(inputSource)
		
	elsif header=='CoinSpark Genesis Tests Input'
		ProcessGenesisTests(inputSource)
		
	elsif header=='CoinSpark Transfer Tests Input'
		ProcessTransferTests(inputSource)
		
	elsif header=='CoinSpark MessageHash Tests Input'
		ProcessMessageHashTests(inputSource)
		
	end
end


def ProcessAddressTests(inputSource)
	print("CoinSpark Address Tests Output\n\n")
	
	while true
		inputLine=GetNextInputLine(inputSource)
		break if inputLine==nil
		
		address=CoinSparkAddress.new
		
		if address.decode(inputLine)
			puts(address.toString)
		else
			abort("Failed to decode address: "+inputLine)
		end
		
		encoded=address.encode
		
		abort("Encode address mismatch: "+encoded+" should be "+inputLine) if encoded!=inputLine
			
		abort("Failed to match address to itself!") if !address.match(address)
	end
end


def ProcessAssetRefTests(inputSource)
	print("CoinSpark AssetRef Tests Output\n\n")
	
	while true
		inputLine=GetNextInputLine(inputSource)
		break if inputLine==nil
	
		assetRef=CoinSparkAssetRef.new
	
		if assetRef.decode(inputLine)
			puts(assetRef.toString)
		else
			abort("Failed to decode AssetRef: "+inputLine)
		end
		
		encoded=assetRef.encode
	
		abort("Encode AssetRef mismatch: "+encoded+" should be "+inputLine) if encoded!=inputLine
	
		abort("Failed to match assetRef to itself!") if !assetRef.match(assetRef)
	end
end


def ProcessScriptTests(inputSource)
	print("CoinSpark Script Tests Output\n\n")
	
	while true
		inputLines=GetNextInputLines(inputSource, 4)
		break if inputLines==nil
			
		countInputs, countOutputs, scriptPubKeyHex, dummy = inputLines
		
		metadata=CoinSparkScriptToMetadata(scriptPubKeyHex, true)
		abort("Could not decode script metadata: "+scriptPubKeyHex) if metadata==nil
			
		# Read in the different types of metadata
		
		genesis=CoinSparkGenesis.new
		hasGenesis=genesis.decode(metadata)
		
		paymentRef=CoinSparkPaymentRef.new
		hasPaymentRef=paymentRef.decode(metadata)
		
		transfers=CoinSparkTransferList.new
		hasTransfers=transfers.decode(metadata, countInputs, countOutputs)>0
		
		message=CoinSparkMessage.new
		hasMessage=message.decode(metadata, countOutputs)
		
		# Output the toString()s
		
		puts(genesis.toString) if hasGenesis
		
		puts(paymentRef.toString) if hasPaymentRef
		
		puts(transfers.toString) if hasTransfers
		
		puts(message.toString) if hasMessage
			
		# Re-encode
		
		testMetadata=''
		testMetadataMaxLen=metadata.length
		nextMetadataMaxLen=testMetadataMaxLen
		
		encodeOrder=['genesis', 'paymentRef', 'transfers', 'message']
		
		for encodeField in encodeOrder
			triedNextMetadata=false
			
			if encodeField=='genesis'
				if hasGenesis
					nextMetadata=genesis.encode(nextMetadataMaxLen)
					triedNextMetadata=true
				end
			
			elsif encodeField=='paymentRef'
				if hasPaymentRef
					nextMetadata=paymentRef.encode(nextMetadataMaxLen)
					triedNextMetadata=true
				end
			
			elsif encodeField=='transfers'
				if hasTransfers
					nextMetadata=transfers.encode(countInputs, countOutputs, nextMetadataMaxLen)
					triedNextMetadata=true
				end
			
			elsif encodeField=='message'
				if hasMessage
					nextMetadata=message.encode(countOutputs, nextMetadataMaxLen)
					triedNextMetadata=true
				end
				
			end
					
			if triedNextMetadata
				abort("Failed to reencode "+encodeField+" metadata!") if nextMetadata==nil
				
				if testMetadata.length>0
					testMetadata=CoinSparkMetadataAppend(testMetadata, testMetadataMaxLen, nextMetadata)
					abort("Insufficient space to append "+encodeField+" metadata!") if testMetadata==nil
				else
					testMetadata=nextMetadata
				end
				
				nextMetadataMaxLen=CoinSparkMetadataMaxAppendLen(testMetadata, testMetadataMaxLen)
			end
		end
				
		# Test other library functions while we are here
		
		if hasGenesis
			abort("Failed to match genesis to itself!") if !genesis.match(genesis, true)
								
			abort("Failed to calculate matching hash length!") if genesis.calcHashLen(metadata.length)!=genesis.assetHashLen
				# assumes that metadata only contains genesis
				
			testGenesis=CoinSparkGenesis.new
			testGenesis.decode(metadata)
			
			rounding=rand(3)-1
			
			testGenesis.setQty(0, 0)
			testGenesis.setQty(genesis.getQty(), rounding)
			
			testGenesis.setChargeFlat(0, 0)
			testGenesis.setChargeFlat(genesis.getChargeFlat(), rounding)
			
			abort("Mismatch on genesis rounding!") if !genesis.match(testGenesis, false)				
		end
				
		if hasPaymentRef
			abort("Failed to match paymentRef to itself!") if !paymentRef.match(paymentRef)
		end
				
		if hasTransfers
			abort("Failed to strictly match transfers to itself!") if !transfers.match(transfers, true)
			abort("Failed to leniently match transfers to itself!") if !transfers.match(transfers, false)
		end
				
		if hasMessage
			abort("Failed to strictly match message to itself!") if !message.match(message, true)
			abort("Failed to leniently match message to itself!") if !message.match(message, false)
				
			messageEncode=message.encode(countOutputs, metadata.length) # encode on its own to check calcHashLen()
			
			abort("Failed to calculate matching message hash length!") if message.calcHashLen(countOutputs, messageEncode.length)!=message.hashLen
		end
				
		# Compare to the original
		
		encoded=CoinSparkMetadataToScript(testMetadata, true)
		
		abort("Encode metadata mismatch: "+encoded+" should be "+scriptPubKeyHex) if encoded!=scriptPubKeyHex
		
		checkMetadata=CoinSparkScriptToMetadata(CoinSparkMetadataToScript(testMetadata, false), false)
		
		abort("Binary metadata to/from script mismatch!") if checkMetadata!=testMetadata
	end
end		


def ProcessAssetHashTests(inputSource)
	print("CoinSpark AssetHash Tests Output\n\n")
	
	while true
		inputLines=GetNextInputLines(inputSource, 10)
		break if inputLines==nil
			
		name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent, dummy = inputLines
		
		contractContent.force_encoding('BINARY')
		
		hash=CoinSparkCalcAssetHash(name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent)
		
		puts(hash.unpack("H*")[0].upcase)
	end
end


def ProcessGenesisTests(inputSource)
	print("CoinSpark Genesis Tests Output\n\n")
	
	while true
		inputLines=GetNextInputLines(inputSource, 7)
		break if inputLines==nil
	
		# Break apart and decode the input lines
		
		firstSpentTxId, firstSpentVout, metadataHex, outputsSatoshisString, outputsRegularString, feeSatoshis, dummy = inputLines

		genesis=CoinSparkGenesis.new
		abort("Failed to decode genesis metadata: "+metadataHex) if !genesis.decode([metadataHex].pack('H*'))
			
		outputsSatoshis=outputsSatoshisString.split(',')
		outputsRegular=outputsRegularString.split(',').map{ |outputRegular| outputRegular.to_i!=0 }	
		countOutputs=outputsSatoshis.length
	
		validFeeSatoshis=genesis.calcMinFee(outputsSatoshis, outputsRegular)
			
		# Perform the genesis calculation
		
		if feeSatoshis.to_i>=validFeeSatoshis
			outputBalances=genesis.apply(outputsRegular)
		else
			outputBalances=[0]*countOutputs
		end
			
		# Output the results
		
		printf("%.0f # transaction fee satoshis to be valid\n", validFeeSatoshis)
		puts(outputBalances.map{ |outputBalance| sprintf("%.0f", outputBalance) }.join(',')+" # units of the asset in each output")
		puts(genesis.calcAssetURL(firstSpentTxId, firstSpentVout)+" # asset web page URL\n\n")
	end
end
		
		
def ProcessTransferTests(inputSource)
	print("CoinSpark Transfer Tests Output\n\n")
	
	while true
		inputLines=GetNextInputLines(inputSource, 8)
		break if inputLines==nil
	
		# Break apart and decode the input lines
		
		(genesisMetadataHex, assetRefString, transfersMetadataHex, inputBalancesString,
			outputsSatoshisString, outputsRegularString, feeSatoshis, dummy) = inputLines

		genesis=CoinSparkGenesis.new
		abort("Failed to decode genesis metadata: "+genesisMetadataHex) if !genesis.decode([genesisMetadataHex].pack('H*'))
			
		assetRef=CoinSparkAssetRef.new
		abort("Failed to decode asset reference: assetRefString") if !assetRef.decode(assetRefString)
			
		inputBalances=inputBalancesString.split(',')
		outputsSatoshis=outputsSatoshisString.split(',')
		outputsRegular=outputsRegularString.split(',').map{ |outputRegular| outputRegular.to_i!=0 }	
		
		countInputs=inputBalances.length
		countOutputs=outputsSatoshis.length
	
		transfers=CoinSparkTransferList.new
		if !transfers.decode([transfersMetadataHex].pack('H*'), countInputs, countOutputs)
			abort("Failed to decode transfers metadata: "+transfersMetadataHex)
		end
		validFeeSatoshis=transfers.calcMinFee(countInputs, outputsSatoshis, outputsRegular)
			
		# Perform the transfer calculation and get default flags
		
		if feeSatoshis.to_i>=validFeeSatoshis
			outputBalances=transfers.apply(assetRef, genesis, inputBalances, outputsRegular)
		else
			outputBalances=transfers.applyNone(assetRef, genesis, inputBalances, outputsRegular)
		end
			
		outputsDefault=transfers.defaultOutputs(countInputs, outputsRegular)
		
		# Output the results
		
		printf("%.0f # transaction fee satoshis to be valid\n", validFeeSatoshis)
		puts(outputBalances.map{ |outputBalance| sprintf("%.0f", outputBalance) }.join(',')+" # units of this asset in each output")
		puts(outputsDefault.map{ |outputDefault| outputDefault ? '1' : '0' }.join(',')+" # boolean flags whether each output is in a default route\n\n")
		
		# Test the net and gross calculations using the input balances as example net values
	
		for inputBalance in inputBalances
			testGrossBalance=genesis.calcGross(inputBalance)
			testNetBalance=genesis.calcNet(testGrossBalance)
			
			if inputBalance.to_i!=testNetBalance
				abort(sprintf("Net to gross to net mismatch: %.0f -> %.0f -> %.0f!", inputBalance.to_i, testGrossBalance, testNetBalance))
			end
		end
	end
end


def ProcessMessageHashTests(inputSource)
	print("CoinSpark MessageHash Tests Output\n\n")
		
	while true
		inputLines=GetNextInputLines(inputSource, 2)
		break if inputLines==nil
			
		salt, countParts = inputLines

		salt=salt.force_encoding('BINARY')
		countParts=countParts.to_i
		
		inputLines=GetNextInputLines(inputSource, 3*countParts+1)
		break if inputLines==nil
		
		messageParts=[]
		while (messageParts.length<countParts) && (inputLines.length>0)
			messageParts.push({
				'mimeType' => inputLines.shift,
				'fileName' => inputLines.shift,
				'content' => inputLines.shift.force_encoding('BINARY')
			})
		end
			
		hash=CoinSparkCalcMessageHash(salt, messageParts)
		
		puts(hash.unpack("H*")[0].upcase)
	end
end


# Main entry point for code
	
abort("No CoinSpark test input file was specified\n") if ARGV.length<1

inputFileName=ARGV[0]

if File.file?(inputFileName)
	inputSource=File.open(inputFileName, 'r')
	ProcessInputContents(inputSource)
	inputSource.close
else
	abort("The CoinSpark test input file was wrong\n")
end
