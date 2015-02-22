<?php

/*
 * CoinSpark 2.1 - PHP test suite
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


	error_reporting(E_ALL | E_STRICT);
	
	require_once 'coinspark.php';
	
	$inputFileName=@$argv[1];

	if (file_exists($inputFileName)) {
		$inputSource=fopen($inputFileName, 'rb');
		ProcessInputContents($inputSource);
		fclose($inputSource);

	} else
		echo "No CoinSpark test input file was specified\n\n";
 

	function GetNextInputLine($inputSource)
	{
		$inputLine=fgets($inputSource, 65536);
		if (!strlen($inputLine)) // false or empty string both indicate end of file
			return null;
		
		$inputLine=rtrim($inputLine, "\n");

		$hashPos=strpos($inputLine, ' # ');
		if ($hashPos!==false)
			$inputLine=substr($inputLine, 0, $hashPos);
		
		return $inputLine;
	}
	
	
	function GetNextInputLines($inputSource, $countLines)
	{
		$inputLines=array();
		
		while ($countLines-->0) {
			$inputLine=GetNextInputLine($inputSource);
			if (!isset($inputLine))
				return null;
				
			$inputLines[]=$inputLine;
		}
		
		return $inputLines;
	}
	
	
	function ProcessInputContents($inputSource)
	{
		$header=GetNextInputLine($inputSource);
		GetNextInputLine($inputSource); // discard blank line

		switch ($header) {
			case 'CoinSpark Address Tests Input':
				ProcessAddressTests($inputSource);
				break;

			case 'CoinSpark AssetRef Tests Input':
				ProcessAssetRefTests($inputSource);
				break;
				
			case 'CoinSpark Script Tests Input':
				ProcessScriptTests($inputSource);
				break;
				
			case 'CoinSpark AssetHash Tests Input':
				ProcessAssetHashTests($inputSource);
				break;
				
			case 'CoinSpark Genesis Tests Input':
				ProcessGenesisTests($inputSource);
				break;
				
			case 'CoinSpark Transfer Tests Input':
				ProcessTransferTests($inputSource);
				break;
				
			case 'CoinSpark MessageHash Tests Input':
				ProcessMessageHashTests($inputSource);
				break;
		}
	}


	function ProcessAddressTests($inputSource)
	{
		echo "CoinSpark Address Tests Output\n\n";
		
		while (true) {
			$inputLine=GetNextInputLine($inputSource);
			if (!isset($inputLine))
				break;
				
			$address=new CoinSparkAddress();
			
			if ($address->decode($inputLine))
				echo $address->toString();
			else
				die("Failed to decode address: $inputLine\n");
				
			$encoded=$address->encode();
			
			if ($encoded!=$inputLine)
				die("Encode address mismatch: $encoded should be $inputLine\n");
				
			if (!$address->match($address))
				die("Failed to match address to itself!\n");
		}
	}


	function ProcessAssetRefTests($inputSource)
	{
		echo "CoinSpark AssetRef Tests Output\n\n";
	
		while (true) {
			$inputLine=GetNextInputLine($inputSource);
			if (!isset($inputLine))
				break;

			$assetRef=new CoinSparkAssetRef();
		
			if ($assetRef->decode($inputLine))
				echo $assetRef->toString();
			else
				die("Failed to decode AssetRef: $inputLine\n");
			
			$encoded=$assetRef->encode();
		
			if ($encoded!=$inputLine)
				die("Encode AssetRef mismatch: $encoded should be $inputLine\n");
				
			if (!$assetRef->match($assetRef))
				die("Failed to match assetRef to itself!\n");
		}
	}
	
	
	function ProcessScriptTests($inputSource)
	{
		echo "CoinSpark Script Tests Output\n\n";
		
		while (true) {
			$inputLines=GetNextInputLines($inputSource, 4);
			if (!isset($inputLines))
				break;
				
			list($countInputs, $countOutputs, $scriptPubKeyHex)=$inputLines;

			$metadata=CoinSparkScriptToMetadata($scriptPubKeyHex, true);
			if (!isset($metadata))
				die("Could not decode script metadata: $scriptPubKeyHex\n");
				
		//	Read in the different types of metadata
				
			$genesis=new CoinSparkGenesis();
			$hasGenesis=$genesis->decode($metadata);
			
			$paymentRef=new CoinSparkPaymentRef();
			$hasPaymentRef=$paymentRef->decode($metadata);
			
			$transfers=new CoinSparkTransferList();
			$hasTransfers=$transfers->decode($metadata, $countInputs, $countOutputs);
			
			$message=new CoinSparkMessage();
			$hasMessage=$message->decode($metadata, $countOutputs);
		
		//	Output the toString()s
			
			if ($hasGenesis)
				echo $genesis->toString();
			
			if ($hasPaymentRef)
				echo $paymentRef->toString();
				
			if ($hasTransfers)
				echo $transfers->toString();
				
			if ($hasMessage)
				echo $message->toString();
		
		//	Re-encode
		
			$testMetadata='';
			$testMetadataMaxLen=strlen($metadata);
			$nextMetadataMaxLen=$testMetadataMaxLen;
			
			$encodeOrder=array('genesis', 'paymentRef', 'transfers', 'message');
			
			foreach ($encodeOrder as $encodeField) {
				$triedNextMetadata=false;
				
				switch ($encodeField) {
					case 'genesis':
						if ($hasGenesis) {
							$nextMetadata=$genesis->encode($nextMetadataMaxLen);
							$triedNextMetadata=true;
						}
						break;
					
					case 'paymentRef':
						if ($hasPaymentRef) {
							$nextMetadata=$paymentRef->encode($nextMetadataMaxLen);
							$triedNextMetadata=true;
						}
						break;
					
					case 'transfers':
						if ($hasTransfers) {
							$nextMetadata=$transfers->encode($countInputs, $countOutputs, $nextMetadataMaxLen);
							$triedNextMetadata=true;
						}
						break;
						
					case 'message':
						if ($hasMessage) {
							$nextMetadata=$message->encode($countOutputs, $nextMetadataMaxLen);
							$triedNextMetadata=true;
						}
						break;
				}
				
				if ($triedNextMetadata) {
					if (!isset($nextMetadata))
						die("Failed to reencode $encodeField metadata!\n");

					if (strlen($testMetadata)) {
						$testMetadata=CoinSparkMetadataAppend($testMetadata, $testMetadataMaxLen, $nextMetadata);
						if (!isset($testMetadata))
							die("Insufficient space to append $encodeField metadata!\n");

					} else
						$testMetadata=$nextMetadata;
					
					$nextMetadataMaxLen=CoinSparkMetadataMaxAppendLen($testMetadata, $testMetadataMaxLen);
				}
			}
			
		//	Test other library functions while we are here
		
			if ($hasGenesis) {
				if (!$genesis->match($genesis, true))
					die("Failed to match genesis to itself!\n");
				
				if ($genesis->calcHashLen(strlen($metadata))!=$genesis->assetHashLen) // assumes that metadata only contains genesis
					die("Failed to calculate matching asset hash length!\n");
				
				$testGenesis=new CoinSparkGenesis();
				$testGenesis->decode($metadata);
				
				$rounding=rand(0, 2)-1;
				
				$testGenesis->setQty(0, 0);
				$testGenesis->setQty($genesis->getQty(), $rounding);
				
				$testGenesis->setChargeFlat(0, 0);
				$testGenesis->setChargeFlat($genesis->getChargeFlat(), $rounding);
				
				if (!$genesis->match($testGenesis, false))
					die("Mismatch on genesis rounding!\n");
			}
			
			if ($hasPaymentRef)
				if (!$paymentRef->match($paymentRef))
					die("Failed to match paymentRef to itself!\n");
			
			if ($hasTransfers) {
				if (!$transfers->match($transfers, true))
					die("Failed to strictly match transfers to itself!\n");

				if (!$transfers->match($transfers, false))
					die("Failed to leniently match transfers to itself!\n");
			}
			
			if ($hasMessage) {
				if (!$message->match($message, true))
					die("Failed to strictly match message to itself!\n");

				if (!$message->match($message, false))
					die("Failed to leniently match message to itself!\n");
					
				$messageEncode=$message->encode($countOutputs, strlen($metadata)); // encode on its own to check calcHashLen()
				
				if ($message->calcHashLen($countOutputs, strlen($messageEncode))!=$message->hashLen)
					die("Failed to calculate matching message hash length!\n");
			}
			
		//	Compare to the original
			
			$encoded=CoinSparkMetadataToScript($testMetadata, true);
			
			if ($encoded!=$scriptPubKeyHex)
				die("Encode metadata mismatch: $encoded should be $scriptPubKeyHex\n");
				
			$checkMetadata=CoinSparkScriptToMetadata(CoinSparkMetadataToScript($testMetadata, false), false);
			
			if ($checkMetadata!=$testMetadata)
				die("Binary metadata to/from script mismatch!\n");
		}
	}
	
	
	function ProcessAssetHashTests($inputSource)
	{
		echo "CoinSpark AssetHash Tests Output\n\n";
		
		while (true) {
			$inputLines=GetNextInputLines($inputSource, 10);
			if (!isset($inputLines))
				break;
				
			list($name, $issuer, $description, $units, $issueDate, $expiryDate, $interestRate, $multiple, $contractContent)=$inputLines;
		
			$hash=CoinSparkCalcAssetHash($name, $issuer, $description, $units, $issueDate, $expiryDate, $interestRate, $multiple, $contractContent);
			
			echo strtoupper(bin2hex($hash))."\n";
		}
	}
	
	
	function ProcessGenesisTests($inputSource)
	{
		echo "CoinSpark Genesis Tests Output\n\n";
		
		while (true) {
			$inputLines=GetNextInputLines($inputSource, 7);
			if (!isset($inputLines))
				break;
		
		//	Break apart and decode the input lines
			
			list($firstSpentTxId, $firstSpentVout, $metadataHex, $outputsSatoshisString, $outputsRegularString, $feeSatoshis)=$inputLines;

			$genesis=new CoinSparkGenesis();
			if (!$genesis->decode(pack('H*', $metadataHex)))
				die("Failed to decode genesis metadata: $metadataHex\n");
				
			$outputsSatoshis=explode(',', $outputsSatoshisString);
			$outputsRegular=explode(',', $outputsRegularString);			
			$countOutputs=count($outputsSatoshis);
		
			$validFeeSatoshis=$genesis->calcMinFee($outputsSatoshis, $outputsRegular);
				
		//	Perform the genesis calculation
			
			if ($feeSatoshis>=$validFeeSatoshis)
				$outputBalances=$genesis->apply($outputsRegular);
			else
				$outputBalances=array_fill(0, $countOutputs, 0);
				
		//	Output the results
			
			echo sprintf("%.0f", $validFeeSatoshis)." # transaction fee satoshis to be valid\n";
			foreach ($outputBalances as $outputIndex => $outputBalance)
				echo ($outputIndex ? ',' : '').sprintf("%.0f", $outputBalance);
			echo " # units of the asset in each output\n";
			echo $genesis->calcAssetURL($firstSpentTxId, $firstSpentVout)." # asset web page URL\n\n";
		}
	}

	
	function ProcessTransferTests($inputSource)
	{
		echo "CoinSpark Transfer Tests Output\n\n";
		
		while (true) {
			$inputLines=GetNextInputLines($inputSource, 8);
			if (!isset($inputLines))
				break;
		
		//	Break apart and decode the input lines
			
			list($genesisMetadataHex, $assetRefString, $transfersMetadataHex, $inputBalancesString,
				$outputsSatoshisString, $outputsRegularString, $feeSatoshis)=$inputLines;

			$genesis=new CoinSparkGenesis();
			if (!$genesis->decode(pack('H*', $genesisMetadataHex)))
				die("Failed to decode genesis metadata: $genesisMetadataHex\n");
				
			$assetRef=new CoinSparkAssetRef();
			if (!$assetRef->decode($assetRefString))
				die("Failed to decode asset reference: $assetRefString\n");
				
			$inputBalances=explode(',', $inputBalancesString);
			$outputsSatoshis=explode(',', $outputsSatoshisString);
			$outputsRegular=explode(',', $outputsRegularString);
			
			$countInputs=count($inputBalances);
			$countOutputs=count($outputsSatoshis);
		
			$transfers=new CoinSparkTransferList();
			if (!$transfers->decode(pack('H*', $transfersMetadataHex), $countInputs, $countOutputs))
				die("Failed to decode transfers metadata: $transfersMetadataHex\n");
			$validFeeSatoshis=$transfers->calcMinFee($countInputs, $outputsSatoshis, $outputsRegular);
				
		//	Perform the transfer calculation and get default flags
			
			if ($feeSatoshis>=$validFeeSatoshis)
				$outputBalances=$transfers->apply($assetRef, $genesis, $inputBalances, $outputsRegular);
			else
				$outputBalances=$transfers->applyNone($assetRef, $genesis, $inputBalances, $outputsRegular);
				
			$outputsDefault=$transfers->defaultOutputs($countInputs, $outputsRegular);
				
		//	Output the results
			
			echo sprintf("%.0f", $validFeeSatoshis)." # transaction fee satoshis to be valid\n";

			foreach ($outputBalances as $outputIndex => $outputBalance)
				echo ($outputIndex ? ',' : '').sprintf("%.0f", $outputBalance);
			echo " # units of this asset in each output\n";
			
			foreach ($outputsDefault as $outputIndex => $outputDefault)
				echo ($outputIndex ? ',' : '').($outputDefault ? '1' : '0');
			echo " # boolean flags whether each output is in a default route\n\n";
			
		//	Test the net and gross calculations using the input balances as example net values
		
			foreach ($inputBalances as $inputBalance) {
				$testGrossBalance=$genesis->calcGross($inputBalance);
				$testNetBalance=$genesis->calcNet($testGrossBalance);
				
				if ($inputBalance!=$testNetBalance)
					die(sprintf("Net to gross to net mismatch: %.0f -> %.0f -> %.0f!\n", $inputBalance, $testGrossBalance, $testNetBalance));
			}
		}
	}
	
	
	function ProcessMessageHashTests($inputSource)
	{
		echo "CoinSpark MessageHash Tests Output\n\n";
		
		while (true) {
			$inputLines=GetNextInputLines($inputSource, 2);
			if (!isset($inputLines))
				break;
				
			list($salt, $countParts)=$inputLines;
			
			$inputLines=GetNextInputLines($inputSource, 3*$countParts+1);
			if (!isset($inputLines))
				break;
				
			$messageParts=array();
			while ((count($messageParts)<$countParts) && count($inputLines))
				$messageParts[]=array(
					'mimeType' => array_shift($inputLines),
					'fileName' => array_shift($inputLines),
					'content' => array_shift($inputLines),
				);
			
			$hash=CoinSparkCalcMessageHash($salt, $messageParts);
			
			echo strtoupper(bin2hex($hash))."\n";
		}
	}