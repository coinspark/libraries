/*
 * CoinSpark 2.1 - Javascript test suite
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


function GetNextInputLine(inputSource)
{
	if (inputSource.length) {
		var inputLine=inputSource.shift();
	
		var hashPos=inputLine.indexOf(' # ');
		if (hashPos>=0)
			inputLine=inputLine.substring(0, hashPos);

	} else
		var inputLine=null;
	
	return inputLine;
}


function GetNextInputLines(inputSource, countLines)
{
	var inputLines=[];
	
	while (countLines-->0) {
		var inputLine=GetNextInputLine(inputSource);
		if (inputLine===null)
			return null;
			
		inputLines[inputLines.length]=inputLine;
	}
	
	return inputLines;
}


function ProcessInputText(inputText)
{
	return ProcessInputLines(inputText.replace(/\r\n/, "\n").replace(/\n$/, '').split("\n"));
}


function ProcessInputLines(inputSource)
{
	var header=GetNextInputLine(inputSource);
	GetNextInputLine(inputSource); // discard blank line
	
	var result=null;
	
	switch (header) {
		case 'CoinSpark Address Tests Input':
			result=ProcessAddressTests(inputSource);
			break;

		case 'CoinSpark AssetRef Tests Input':
			result=ProcessAssetRefTests(inputSource);
			break;
			
		case 'CoinSpark Script Tests Input':
			result=ProcessScriptTests(inputSource);
			break;
			
		case 'CoinSpark AssetHash Tests Input':
			result=ProcessAssetHashTests(inputSource);
			break;
			
		case 'CoinSpark Genesis Tests Input':
			result=ProcessGenesisTests(inputSource);
			break;
			
		case 'CoinSpark Transfer Tests Input':
			result=ProcessTransferTests(inputSource);
			break;
			
		case 'CoinSpark MessageHash Tests Input':
			result=ProcessMessageHashTests(inputSource);
			break;
	}
	
	return result;
}


function ProcessAddressTests(inputSource)
{
	var result="CoinSpark Address Tests Output\n\n";
	
	while (true) {
		var inputLine=GetNextInputLine(inputSource);
		if (inputLine===null)
			break;
			
		var address=new CoinSparkAddress();
		
		if (address.decode(inputLine))
			result+=address.toString();
		else
			return result+"Failed to decode address: "+inputLine+"\n";
			
		var encoded=address.encode();
		
		if (encoded!=inputLine)
			return result+"Encode address mismatch: "+encoded+" should be "+inputLine+"\n";
			
		if (!address.match(address))
			return result+"Failed to match address to itself!\n";
	}
	
	return result;
}


function ProcessAssetRefTests(inputSource)
{
	var result="CoinSpark AssetRef Tests Output\n\n";
	
	while (true) {
		var inputLine=GetNextInputLine(inputSource);
		if (inputLine===null)
			break
			
		var assetRef=new CoinSparkAssetRef();
		
		if (assetRef.decode(inputLine))
			result+=assetRef.toString();
		else
			return result+"Failed to decode AssetRef: "+inputLine+"\n";
			
		var encoded=assetRef.encode();
		
		if (encoded!=inputLine)
			return result+"Encode AssetRef mismatch: "+encoded+" should be "+inputLine+"\n";
			
		if (!assetRef.match(assetRef))
			return result+"Failed to match assetRef to itself!\n";
	}
	
	return result;
}


function ProcessScriptTests(inputSource)
{
	var result="CoinSpark Script Tests Output\n\n";
	
	while (true) {
		var inputLines=GetNextInputLines(inputSource, 4);
		if (!inputLines)
			break;
		
		var countInputs=inputLines[0];
		var countOutputs=inputLines[1];
		var scriptPubKeyHex=inputLines[2];

		var metadata=CoinSparkScriptToMetadata(scriptPubKeyHex, true);
		if (!metadata)
			return result+"Could not decode script metadata: "+scriptPubKeyHex+"\n";
			
	//	Read in the different types of metadata
			
		var genesis=new CoinSparkGenesis();
		var hasGenesis=genesis.decode(metadata);
		
		var paymentRef=new CoinSparkPaymentRef();
		var hasPaymentRef=paymentRef.decode(metadata);
		
		var transfers=new CoinSparkTransferList();
		var hasTransfers=transfers.decode(metadata, countInputs, countOutputs);
		
		var message=new CoinSparkMessage();
		var hasMessage=message.decode(metadata, countOutputs);
	
	//	Append the toString()s
		
		if (hasGenesis)
			result+=genesis.toString();
		
		if (hasPaymentRef)
			result+=paymentRef.toString();
			
		if (hasTransfers)
			result+=transfers.toString();
			
		if (hasMessage)
			result+=message.toString();
	
	//	Re-encode
	
		var testMetadata=[];
		var nextMetadata=null;
		var testMetadataMaxLen=metadata.length;
		var nextMetadataMaxLen=testMetadataMaxLen;
		
		var encodeOrder=['genesis', 'paymentRef', 'transfers', 'message'];
		
		for (encodeIndex=0; encodeIndex<encodeOrder.length; encodeIndex++) {
			var encodeField=encodeOrder[encodeIndex];
			var triedNextMetadata=false;
			
			switch (encodeField) {
				case 'genesis':
					if (hasGenesis) {
						nextMetadata=genesis.encode(nextMetadataMaxLen);
						triedNextMetadata=true;
					}
					break;
				
				case 'paymentRef':
					if (hasPaymentRef) {
						nextMetadata=paymentRef.encode(nextMetadataMaxLen);
						triedNextMetadata=true;
					}
					break;
				
				case 'transfers':
					if (hasTransfers) {
						nextMetadata=transfers.encode(countInputs, countOutputs, nextMetadataMaxLen);
						triedNextMetadata=true;
					}
					break;
					
				case 'message':
					if (hasMessage) {
						nextMetadata=message.encode(countOutputs, nextMetadataMaxLen);
						triedNextMetadata=true;
					}
					break;
			}
			
			if (triedNextMetadata) {
				if (!nextMetadata)
					return result+"Failed to reencode "+encodeField+" metadata!\n";

				if (testMetadata.length) {
					testMetadata=CoinSparkMetadataAppend(testMetadata, testMetadataMaxLen, nextMetadata);
					
					if (!testMetadata)
						return result+"Insufficient space to append "+encodeField+" metadata!\n";
				} else
					testMetadata=nextMetadata;
				
				nextMetadataMaxLen=CoinSparkMetadataMaxAppendLen(testMetadata, testMetadataMaxLen);
			}
		}
		
	//	Test other library functions while we are here
	
		if (hasGenesis) {
			if (!genesis.match(genesis, true))
				return result+"Failed to match genesis to itself!\n";
			
			if (genesis.calcHashLen(metadata.length)!=genesis.assetHashLen) // assumes that metadata only contains genesis
				return result+"Failed to calculate matching hash length!\n";
			
			var testGenesis=new CoinSparkGenesis();
			testGenesis.decode(metadata);
			
			var rounding=Math.floor(Math.random()*3)-1;
			
			testGenesis.setQty(0, 0);
			testGenesis.setQty(genesis.getQty(), rounding);
			
			testGenesis.setChargeFlat(0, 0);
			testGenesis.setChargeFlat(genesis.getChargeFlat(), rounding);
			
			if (!genesis.match(testGenesis, false))
				return result+"Mismatch on genesis rounding!\n";
		}
		
		if (hasPaymentRef)
			if (!paymentRef.match(paymentRef))
				return result+"Failed to match paymentRef to itself!\n";
		
		if (hasTransfers) {
			if (!transfers.match(transfers, true))
				return result+"Failed to strictly match transfers to itself!\n";

			if (!transfers.match(transfers, false))
				return result+"Failed to leniently match transfers to itself!\n";
		}
		
		if (hasMessage) {
			if (!message.match(message, true))
				return result+"Failed to strictly match message to itself!\n";

			if (!message.match(message, false))
				return result+"Failed to leniently match message to itself!\n";
			
			var messageEncode=message.encode(countOutputs, metadata.length); // encode on its own to check calcHashLen()

			if (message.calcHashLen(countOutputs, messageEncode.length)!=message.hashLen)
				return result+"Failed to calculate matching message hash length!\n";
		}
		
	//	Compare to the original
		
		var encoded=CoinSparkMetadataToScript(testMetadata, true);
		
		if (encoded!=scriptPubKeyHex)
			return result+"Encode metadata mismatch: "+encoded+" should be "+scriptPubKeyHex+"\n";
			
		var checkMetadata=CoinSparkScriptToMetadata(CoinSparkMetadataToScript(testMetadata, false), false);
		
		if (checkMetadata.toString()!=testMetadata.toString())
			return result+"Binary metadata to/from script mismatch!\n";
	}
	
	return result;
}


function ProcessAssetHashTests(inputSource)
{
	var result="CoinSpark AssetHash Tests Output\n\n";
	
	while (true) {
		var inputLines=GetNextInputLines(inputSource, 10);
		if (!inputLines)
			break;
			
		var name=inputLines[0];
		var issuer=inputLines[1];
		var description=inputLines[2];
		var units=inputLines[3];
		var issueDate=inputLines[4];
		var expiryDate=inputLines[5];
		var interestRate=inputLines[6];
		var multiple=inputLines[7];
		var contractContent=CoinSparkStringToUint8ArrayUTF8(inputLines[8]);
			
		var hash=CoinSparkCalcAssetHash(name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contractContent);
		
		result+=CoinSparkUint8ArrayToHex(hash)+"\n";
	}
	
	return result;
}


function ProcessGenesisTests(inputSource)
{
	var result="CoinSpark Genesis Tests Output\n\n";
	
	while (true) {
		var inputLines=GetNextInputLines(inputSource, 7);
		if (!inputLines)
			break;
	
	//	Break apart and decode the input lines
		
		var firstSpentTxId=inputLines[0];
		var firstSpentVout=inputLines[1];
		var metadataHex=inputLines[2];
		var outputsSatoshisString=inputLines[3];
		var outputsRegularString=inputLines[4];
		var feeSatoshis=inputLines[5];
		
		var genesis=new CoinSparkGenesis();
		if (!genesis.decode(CoinSparkHexToUint8Array(metadataHex)))
			return result+"Failed to decode genesis metadata: "+metadataHex+"\n";
			
		var outputsSatoshis=outputsSatoshisString.split(',');
		var outputsRegular=outputsRegularString.split(',');	
		var countOutputs=outputsSatoshis.length;
		
		for (var outputIndex=0; outputIndex<outputsRegular.length; outputIndex++)
			outputsRegular[outputIndex]=outputsRegular[outputIndex]>0; // because in Javascript, "0" is true
	
		var validFeeSatoshis=genesis.calcMinFee(outputsSatoshis, outputsRegular);
			
	//	Perform the genesis calculation
		
		if (feeSatoshis>=validFeeSatoshis)
			var outputBalances=genesis.apply(outputsRegular);
		else
			var outputBalances=CoinSparkArrayFill(0, countOutputs, 0);
			
	//	Output the results
		
		result+=validFeeSatoshis+" # transaction fee satoshis to be valid\n";
		result+=outputBalances.join(',')+" # units of the asset in each output\n";
		result+=genesis.calcAssetURL(firstSpentTxId, firstSpentVout)+" # asset web page URL\n\n";
	}
	
	return result;
}


function ProcessTransferTests(inputSource)
{
	var result="CoinSpark Transfer Tests Output\n\n";
	
	while (true) {
		var inputLines=GetNextInputLines(inputSource, 8);
		if (!inputLines)
			break;
	
	//	Break apart and decode the input lines
		
		var genesisMetadataHex=inputLines[0];
		var assetRefString=inputLines[1];
		var transfersMetadataHex=inputLines[2];
		var inputBalancesString=inputLines[3];
		var outputsSatoshisString=inputLines[4];
		var outputsRegularString=inputLines[5];
		var feeSatoshis=inputLines[6];
		
		var genesis=new CoinSparkGenesis();
		if (!genesis.decode(CoinSparkHexToUint8Array(genesisMetadataHex)))
			return result+"Failed to decode genesis metadata: "+genesisMetadataHex+"\n";
			
		var assetRef=new CoinSparkAssetRef();
		if (!assetRef.decode(assetRefString))
			return result+"Failed to decode asset reference: "+assetRefString+"\n";
			
		var inputBalances=inputBalancesString.split(',');
		var outputsSatoshis=outputsSatoshisString.split(',');
		var outputsRegular=outputsRegularString.split(',');
		
		for (var outputIndex=0; outputIndex<outputsRegular.length; outputIndex++)
			outputsRegular[outputIndex]=outputsRegular[outputIndex]>0; // because in Javascript, "0" is true
			
		var countInputs=inputBalances.length;
		var countOutputs=outputsSatoshis.length;
	
		var transfers=new CoinSparkTransferList();
		if (!transfers.decode(CoinSparkHexToUint8Array(transfersMetadataHex), countInputs, countOutputs))
			return result+"Failed to decode transfers metadata: "+transfersMetadataHex+"\n";
		var validFeeSatoshis=transfers.calcMinFee(countInputs, outputsSatoshis, outputsRegular);
			
	//	Perform the transfer calculation and get default flags
		
		if (feeSatoshis>=validFeeSatoshis)
			var outputBalances=transfers.apply(assetRef, genesis, inputBalances, outputsRegular);
		else
			var outputBalances=transfers.applyNone(assetRef, genesis, inputBalances, outputsRegular);
			
		var outputsDefault=transfers.defaultOutputs(countInputs, outputsRegular);
			
	//	Output the results
		
		result+=validFeeSatoshis+" # transaction fee satoshis to be valid\n";
		result+=outputBalances.join(',')+" # units of this asset in each output\n";
		
		for (outputIndex=0; outputIndex<outputsDefault.length; outputIndex++)
			result+=(outputIndex ? ',' : '')+(outputsDefault[outputIndex] ? '1' : '0');
		result+=" # boolean flags whether each output is in a default route\n\n";
		
	//	Test the net and gross calculations using the input balances as example net values
	
		for (var inputIndex=0; inputIndex<inputBalances.length; inputIndex++) {
			var inputBalance=inputBalances[inputIndex];
			var testGrossBalance=genesis.calcGross(inputBalance);
			var testNetBalance=genesis.calcNet(testGrossBalance);
			
			if (inputBalance!=testNetBalance)
				return result+"Net to gross to net mismatch: "+inputBalance+" -> "+testGrossBalance+" -> "+testNetBalance+"!\n";
		}
	}
	
	return result;
}


function ProcessMessageHashTests(inputSource)
{
	var result="CoinSpark MessageHash Tests Output\n\n";
	
	while (true) {
		var inputLines=GetNextInputLines(inputSource, 2);
		if (!inputLines)
			break;
			
		var salt=inputLines[0];
		var countParts=inputLines[1];
			
		inputLines=GetNextInputLines(inputSource, 3*countParts+1);
		if (!inputLines)
			break;
			
		var messageParts=[];
		while ((messageParts.length<countParts) && inputLines.length)
			messageParts[messageParts.length]={
				'mimeType': inputLines.shift(),
				'fileName': inputLines.shift(),
				'content': inputLines.shift()
			};
		
		var hash=CoinSparkCalcMessageHash(salt, messageParts);
		
		result+=CoinSparkUint8ArrayToHex(hash)+"\n";
	}
	
	return result;
}