/*
 * CoinSpark 2.1 - Java test suite
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


package org.coinspark.protocol;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Test various components of the CoinSpark library.
 */
public class CoinSparkTest {
    
    
    public enum CoinSparkTestType {

        ADDRESS("A","Addresses", "Address"),
        REFERENCE("R","Asset References", "AssetRef"),
        SCRIPT("S","Script metadata", "Script"),
        HASH("H","Asset Hashes", "AssetHash"),
        GENESIS("G","Genesis calculations", "Genesis"),
        TRANSFER("T","Transfer calculations", "Transfer"),
        MESSAGE("M","Message Hashes", "MessageHash");
        
        private String letter;
        private String text;
        private String suffix;
        
        CoinSparkTestType(String Letter,String Text,String Suffix)
        {
            letter=Letter;
            text=Text;
            suffix=Suffix;
        }
        
        private String getLetter()
        {
            return letter;
        }
        
        private String getText()
        {
            return text;
        }
        
        private String getSuffix()
        {
            return suffix;
        }
        
        public static CoinSparkTestType fromLetter(String Letter) {
            if (Letter != null) {
                Letter = Letter.trim().toUpperCase();
                for (CoinSparkTestType lt : CoinSparkTestType.values()) {
                    if (lt.getLetter().equals(Letter)) {
                        return lt;
                    }
                }
            }
            return null;
        }        
    }    

    private CoinSparkTestType testType;
    private String inputFile;
    private String outputFile;
    private String logFile;
    BufferedReader inputBR;
    FileWriter outputFW;
    FileWriter logFW;
    
    public CoinSparkTest(String Directory, CoinSparkTestType TestType) 
    {
        testType=TestType;
        if(testType != null)
        {
            inputFile=Directory + File.separator +  TestType.getSuffix() + "-Input.txt";
            outputFile=Directory + File.separator +  TestType.getSuffix() + "-Output-Java.txt";
            logFile=Directory + File.separator + TestType.getSuffix() + "-Output-Java.log";
        }
    }
    
    private String getInputLine()
    {
        if(inputBR == null)
            return null;
        
        String line;
        try {
            line=inputBR.readLine();
        } catch (IOException ex) {
            System.out.print(String.format("Cannot read input file %s\n",inputFile));            
            return null;
        }
        
        if(line != null)
        {
            line=("x" + line).trim().substring(1);
            int pos=line.indexOf(" # ");
            if(pos>=0)
            {
                line=line.substring(0, pos);
            }
        }
        
        return line;
    }

    private String [] getInputLines(int count)
    {
        if(count<=0)
        {
            return null;
        }
        String [] result=new String[count];
        for(int i=0;i<count;i++)
        {
            result[i]=getInputLine();
            if(result[i] == null)
            {
                return null;
            }
        }
        return result;
    }
    
    private boolean writeOutputLine(String line)
    {
        return writeOutputLine(line,"\n");
    }
    
    private boolean writeOutput(String line)
    {
        return writeOutputLine(line,"");
    }
    
    private boolean writeOutputLine(String line,String eol)
    {
        try {
            outputFW.write(line + eol);
        } catch (IOException ex) {
            System.out.print(String.format("Cannot write to output file %s\n",inputFile));            
            return false;
        }
        
        return true;
    }
    
    private boolean writeLogLine(String line)
    {
        try {
            logFW.write(line + "\n");
        } catch (IOException ex) {
            System.out.print(String.format("Cannot write to log file %s\n",inputFile));            
            return false;
        }
        
        return true;
    }
    
    private boolean performAddressTest(boolean ExitOnFailure)
    {
        boolean result=true;
        
        String line=getInputLine();
        if((line == null) || !line.equals("CoinSpark Address Tests Input"))
        {
            writeLogLine("Different header line expected, got " + line);
            return false;
        }
        line=getInputLine();
        if((line == null) || (line.length()>0))
        {
            writeLogLine("Empty line expected, got " + line);
            return false;
        }
        
		writeOutputLine("CoinSpark Address Tests Output");
		writeOutputLine("");
		
        while((line=getInputLine()) != null)
        {
            boolean try_again=true;
            boolean this_result=true;
            while(try_again)
            {
                if(this_result)
                {
                    try_again=false;
                }
                CoinSparkAddress address=new CoinSparkAddress();

                if(address.decode(line))
                {
                    writeOutput(address.toString());
                    String encoded=address.encode();
                    if(encoded != null)
                    {
                        int orglen=line.length();
                        int englen=encoded.length();

                        if(!line.equals(encoded))
                        {
                            writeLogLine("Encode address mismatch: " + encoded + " should be " + line);
                            this_result=false;
                        }                            
                    }
                    else
                    {
                        writeLogLine("Failed to encode address " + line);
                        this_result=false;                   
                    }

                    if(!address.match(address))
                    {
                        writeLogLine("Failed to match address to itself! " + line);                  
                        this_result=false;
                    }
                }
                else
                {
                    writeLogLine("Failed to decode address " + line);
                    this_result=false;
                }			
                
                result &= this_result;
                if(!try_again)
                {
                    if(!this_result)
                    {
                        try_again=true;
                    }
                }
                else
                {
                    if(ExitOnFailure)
                    {
                        return result;
                    }
                    else
                    {
                        try_again=false;
                    }
                }
            }
		}
        
        return result;
    }

    private boolean performAssetRefTest(boolean ExitOnFailure)
    {
        boolean result=true;
        
        String line=getInputLine();
        if((line == null) || !line.equals("CoinSpark AssetRef Tests Input"))
        {
            writeLogLine("Different header line expected, got " + line);
            return false;
        }
        line=getInputLine();
        if((line == null) || (line.length()>0))
        {
            writeLogLine("Empty line expected, got " + line);
            return false;
        }
        
		writeOutputLine("CoinSpark AssetRef Tests Output");
		writeOutputLine("");
		
        while((line=getInputLine()) != null)
        {
            boolean try_again=true;
            boolean this_result=true;
            while(try_again)
            {
                if(this_result)
                {
                    try_again=false;
                }
                
                CoinSparkAssetRef assetRef=new CoinSparkAssetRef();
                
                if(assetRef.decode(line))
                {
                    writeOutput(assetRef.toStringInner(true));
                    String encoded=assetRef.encode();
                    if(encoded != null)
                    {
                        if(!line.equals(encoded))
                        {
                            writeLogLine("Encode assetRef mismatch: " + encoded + " should be " + line);
                            this_result=false;
                        }                            
                    }
                    else
                    {
                        writeLogLine("Failed to encode assetRef " + line);
                        this_result=false;                   
                    }

                    if(!assetRef.match(assetRef))
                    {
                        writeLogLine("Failed to match assetRef to itself! " + line);                    
                        this_result=false;
                    }
                }
                else
                {
                    writeLogLine("Failed to decode assetRef " + line);
                    this_result=false;
                }			
                
                result &= this_result;
                if(!try_again)
                {
                    if(!this_result)
                    {
                        try_again=true;
                    }
                }
                else
                {
                    if(ExitOnFailure)
                    {
                        return result;
                    }
                    else
                    {
                        try_again=false;
                    }
                }
            }
		}
        
        return result;
    }
    

    private boolean performGenesisTest(boolean ExitOnFailure)
    {
        boolean result=true;
        
        String line=getInputLine();
        if((line == null) || !line.equals("CoinSpark Genesis Tests Input"))
        {
            writeLogLine("Different header line expected, got " + line);
            return false;
        }
        line=getInputLine();
        if((line == null) || (line.length()>0))
        {
            writeLogLine("Empty line expected, got " + line);
            return false;
        }
        
		writeOutputLine("CoinSpark Genesis Tests Output");
		writeOutputLine("");
		
        String [] lines;
        
        while((lines=getInputLines(7)) != null)
        {
            boolean try_again=true;
            boolean this_result=true;
            while(try_again)
            {
                if(this_result)
                {
                    try_again=false;
                }
                
                String firstSpentTxId=lines[0];
                long firstSpentVout=Long.valueOf(lines[1]);
                String metadataHex=lines[2];
                String outputsSatoshisString=lines[3];
                String outputsRegularString=lines[4];
                long feeSatoshis=Long.valueOf(lines[5]);
                
                CoinSparkGenesis genesis=new CoinSparkGenesis();
                
                if(genesis.decode(metadataHex))
                {
                    String [] outputsSatoshisSplit=outputsSatoshisString.split(",");
                    String [] outputsRegularSplit=outputsRegularString.split(",");
                    int outputCount=outputsSatoshisSplit.length;
                    long [] outputsSatoshis=new long [outputCount];
                    boolean [] outputsRegular=new boolean [outputCount];
                    
                    for(int i=0;i<outputCount;i++)
                    {
                        outputsSatoshis[i]=Long.valueOf(outputsSatoshisSplit[i]);
                        outputsRegular[i]=true;
                        if(outputsRegularSplit[i].equals("0"))outputsRegular[i]=false;
                    }
                    
                    long validFeeSatoshis=genesis.calcMinFee(outputsSatoshis, outputsRegular);
                    
                    long [] outputBalances;
                    if(feeSatoshis >= validFeeSatoshis)
                    {
                        outputBalances=genesis.apply(outputsRegular);
                    }
                    else
                    {
                        outputBalances=new long[outputCount];
                        for(int i=0;i<outputCount;i++)
                        {
                            outputBalances[i]=0;
                        }
                    }
                    
                    writeOutput(String.format("%d # transaction fee satoshis to be valid\n", validFeeSatoshis));
                    for(int i=0;i<outputCount;i++)
                    {
                        writeOutput((i>0 ? "," : "") + String.format("%d", outputBalances[i]));
                    }                    
                    writeOutput(" # units of the asset in each output\n");
                    writeOutput(genesis.calcAssetURL(firstSpentTxId, firstSpentVout) + " # asset web page URL\n\n");
                    
                    String encoded=genesis.encodeToHex(40);
                    if(encoded != null)
                    {
                        if(!metadataHex.equals(encoded))
                        {
                            writeLogLine("Encode genesis mismatch: " + encoded + " should be " + metadataHex);
                            this_result=false;
                        }                            
                    }
                    else
                    {
                        writeLogLine("Failed to encode genesis " + metadataHex);
                        this_result=false;                   
                    }

                    if(!genesis.match(genesis,true))
                    {
                        writeLogLine("Failed to match genesis to itself! " + metadataHex);                    
                        this_result=false;
                    }
                }
                else
                {
                    writeLogLine("Failed to decode genesis " + line);
                    this_result=false;
                }			
                
                result &= this_result;
                if(!try_again)
                {
                    if(!this_result)
                    {
                        try_again=true;
                    }
                }
                else
                {
                    if(ExitOnFailure)
                    {
                        return result;
                    }
                    else
                    {
                        try_again=false;
                    }
                }
            }
		}
        
        return result;
    }
    
    private boolean performScriptTest(boolean ExitOnFailure)
    {
        boolean result=true;
        
        String line=getInputLine();
        if((line == null) || !line.equals("CoinSpark Script Tests Input"))
        {
            writeLogLine("Different header line expected, got " + line);
            return false;
        }
        line=getInputLine();
        if((line == null) || (line.length()>0))
        {
            writeLogLine("Empty line expected, got " + line);
            return false;
        }
        
		writeOutputLine("CoinSpark Script Tests Output");
		writeOutputLine("");
		
        String [] lines;
        
        while((lines=getInputLines(4)) != null)
        {
            boolean try_again=true;
            boolean this_result=true;
            Random rnd=new Random();
            int rounding=rnd.nextInt(3)-1;
            while(try_again)
            {
                if(this_result)
                {
                    try_again=false;
                }
                
                int countInputs=Integer.valueOf(lines[0]);
                int countOutputs=Integer.valueOf(lines[1]);
                String ScriptPubKeyHex=lines[2];

                byte [] metadata=CoinSparkBase.scriptToMetadata(ScriptPubKeyHex);
                                
                CoinSparkGenesis genesis=new CoinSparkGenesis();                
                boolean hasGenesis=genesis.decode(metadata);
                
                CoinSparkPaymentRef paymentRef=new CoinSparkPaymentRef();
                boolean hasPaymentRef=paymentRef.decode(metadata);
                
                CoinSparkTransferList transfers=new CoinSparkTransferList();
                boolean hasTransferList=transfers.decode(metadata, countInputs, countOutputs);

                CoinSparkMessage message=new CoinSparkMessage();
                boolean hasMessage=message.decode(metadata, countOutputs);
                
                if(hasGenesis)
                {
                    writeOutput(genesis.toString());
                }
   
                if(hasPaymentRef)
                {
                    writeOutput(paymentRef.toString());
                }
   
                if(hasTransferList)
                {
                    writeOutput(transfers.toString());
                }
   
                if(hasMessage)
                {
                    writeOutput(message.toString());
                }
   
                byte [] encoded=null;
                byte [] this_encoded;
                int maxMetadataLen=40;
                
                if(hasGenesis)
                {
                    encoded=genesis.encode(maxMetadataLen);
                    if(encoded == null)
                    {
                        writeLogLine("Failed to encode genesis " + ScriptPubKeyHex);
                        this_result=false;                                           
                    }
                    else
                    {
                        maxMetadataLen=CoinSparkBase.metadataMaxAppendLen(encoded, 40);
                    }
                }
                
                if(hasPaymentRef)
                {
                    this_encoded=paymentRef.encode(40);
                    if(this_encoded == null)
                    {
                        writeLogLine("Failed to encode payment reference " + ScriptPubKeyHex);
                        this_result=false;                                           
                    }
                    else
                    {
                        encoded=CoinSparkBase.metadataAppend(encoded, 40, this_encoded);
                        if(encoded == null)
                        {
                            writeLogLine("Failed to append encoded metadata " + ScriptPubKeyHex);
                            this_result=false;                                                                      
                        }
                        else
                        {
                            maxMetadataLen=CoinSparkBase.metadataMaxAppendLen(encoded, 40);
                        }
                    }
                }
                
                if(hasTransferList)
                {
                    this_encoded=transfers.encode(countInputs, countOutputs,maxMetadataLen);
                    if(this_encoded == null)
                    {
                        writeLogLine("Failed to encode transfer list " + ScriptPubKeyHex);
                        this_result=false;                                           
                    }
                    else
                    {
                        encoded=CoinSparkBase.metadataAppend(encoded, 40, this_encoded);
                        if(encoded == null)
                        {
                            writeLogLine("Failed to append encoded metadata " + ScriptPubKeyHex);
                            this_result=false;                                                                      
                        }
                    }
                }
                
                if(hasMessage)
                {
                    this_encoded=message.encode(countOutputs,maxMetadataLen);
                    if(this_encoded == null)
                    {
                        writeLogLine("Failed to encode message " + ScriptPubKeyHex);
                        this_result=false;                                           
                    }
                    else
                    {
                        encoded=CoinSparkBase.metadataAppend(encoded, 40, this_encoded);
                        if(encoded == null)
                        {
                            writeLogLine("Failed to append encoded metadata " + ScriptPubKeyHex);
                            this_result=false;                                                                      
                        }
                    }
                }
                
                String encodedScriptPubKeyHex=CoinSparkBase.metadataToScriptHex(encoded);
                if(!encodedScriptPubKeyHex.equals(ScriptPubKeyHex))
                {
                    writeLogLine("Encode mismatch: " + encodedScriptPubKeyHex + " should be " + ScriptPubKeyHex);
                    this_result=false;
                }                            
                
                if(hasGenesis)
                {
                    if(!genesis.match(genesis,true))
                    {
                        writeLogLine("Failed to match genesis to itself! " + ScriptPubKeyHex);                    
                        this_result=false;
                    }
                    
                    CoinSparkGenesis testGenesis=new CoinSparkGenesis();
                    testGenesis.decode(metadata);
                    
                    
                    
                    testGenesis.setQty(0, 0);
                    testGenesis.setQty(genesis.getQty(), rounding);
                    
                    testGenesis.setChargeFlat(0, 0);
                    testGenesis.setChargeFlat(genesis.getChargeFlat(), rounding);
                    
                    if(!genesis.match(testGenesis,false))
                    {
                        writeLogLine("Mismatch on genesis rounding! " + ScriptPubKeyHex);                    
                        this_result=false;
                    }
                }
                
                if(hasPaymentRef)
                {
                    if(!paymentRef.match(paymentRef))
                    {
                        writeLogLine("Failed to match payment reference to itself! " + ScriptPubKeyHex);                    
                        this_result=false;
                    }                    
                }
                
                if(hasTransferList)
                {
                    if(!transfers.match(transfers,true))
                    {
                        writeLogLine("Failed to strictly match transfer list to itself! " + ScriptPubKeyHex);                    
                        this_result=false;
                    }
                    
                    if(!transfers.match(transfers,false))
                    {
                        writeLogLine("Failed to leniently match transfer list to itself! " + ScriptPubKeyHex);                    
                        this_result=false;
                    }
                }
                
                if(hasMessage)
                {
                    if(!message.match(message,true))
                    {
                        writeLogLine("Failed to strictly match message to itself! " + ScriptPubKeyHex);                    
                        this_result=false;
                    }
                    
                    if(!message.match(message,false))
                    {
                        writeLogLine("Failed to leniently match message to itself! " + ScriptPubKeyHex);                    
                        this_result=false;
                    }
                }
                
                
                result &= this_result;
                if(!try_again)
                {
                    if(!this_result)
                    {
                        try_again=true;
                    }
                }
                else
                {
                    if(ExitOnFailure)
                    {
                        return result;
                    }
                    else
                    {
                        try_again=false;
                    }
                }
            }
		}
        
        return result;
    }
    
    private boolean performTransferTest(boolean ExitOnFailure)
    {
        boolean result=true;
        
        String line=getInputLine();
        if((line == null) || !line.equals("CoinSpark Transfer Tests Input"))
        {
            writeLogLine("Different header line expected, got " + line);
            return false;
        }
        line=getInputLine();
        if((line == null) || (line.length()>0))
        {
            writeLogLine("Empty line expected, got " + line);
            return false;
        }
        
		writeOutputLine("CoinSpark Transfer Tests Output");
		writeOutputLine("");
		
        String [] lines;
        
        while((lines=getInputLines(8)) != null)
        {
            boolean try_again=true;
            boolean this_result=true;
            while(try_again)
            {
                if(this_result)
                {
                    try_again=false;
                }
                
                String genesisMetadataHex=lines[0];
                String assetRefString=lines[1];
                String metadataHex=lines[2];
                String inputBalancesString=lines[3];
                String outputsSatoshisString=lines[4];
                String outputsRegularString=lines[5];
                long feeSatoshis=Long.valueOf(lines[6]);
                
                CoinSparkGenesis genesis=new CoinSparkGenesis();
                
                if(genesis.decode(genesisMetadataHex))
                {
                    CoinSparkAssetRef assetRef=new CoinSparkAssetRef();

                    if(assetRef.decode(assetRefString))
                    {
                        String [] inputBalancesSplit=inputBalancesString.split(",");
                        String [] outputsSatoshisSplit=outputsSatoshisString.split(",");
                        String [] outputsRegularSplit=outputsRegularString.split(",");
                        int inputCount=inputBalancesSplit.length;
                        int outputCount=outputsSatoshisSplit.length;
                        long [] inputBalances=new long [inputCount];
                        long [] outputsSatoshis=new long [outputCount];
                        boolean [] outputsRegular=new boolean [outputCount];

                        for(int i=0;i<inputCount;i++)
                        {
                            inputBalances[i]=Long.valueOf(inputBalancesSplit[i]);
                        }
                        for(int i=0;i<outputCount;i++)
                        {
                            outputsSatoshis[i]=Long.valueOf(outputsSatoshisSplit[i]);
                            outputsRegular[i]=true;
                            if(outputsRegularSplit[i].equals("0"))outputsRegular[i]=false;
                        }
                        
                        CoinSparkTransferList transfers=new CoinSparkTransferList();
                        if(transfers.decode(metadataHex, inputCount, outputCount))
                        {
                            long validFeeSatoshis=transfers.calcMinFee(inputCount, outputsSatoshis, outputsRegular);
                         
                            long [] outputBalances;
                            if(feeSatoshis >= validFeeSatoshis)
                            {
                                outputBalances=transfers.apply(assetRef, genesis, inputBalances, outputsRegular);
                            }
                            else
                            {
                                outputBalances=transfers.applyNone(assetRef, genesis, inputBalances,  outputsRegular);
                            }
                            
                            boolean [] outputsDefault=transfers.defaultOutputs(inputCount, outputsRegular);
                            
                            writeOutput(String.format("%d # transaction fee satoshis to be valid\n", validFeeSatoshis));
                            for(int i=0;i<outputCount;i++)
                            {
                                writeOutput((i>0 ? "," : "") + String.format("%d", outputBalances[i]));
                            }                    
                            writeOutput(" # units of this asset in each output\n");
                            
                            for(int i=0;i<outputCount;i++)
                            {
                                writeOutput((i>0 ? "," : "") + (outputsDefault[i] ? "1" : "0"));
                            }                    
                            writeOutput(" # boolean flags whether each output is in a default route\n\n");
                            
                            for(int i=0;i<inputCount;i++)
                            {
                                long testGrossBalance=genesis.calcGross(inputBalances[i]);
                                long testNetBalance=genesis.calcNet(testGrossBalance);
                                
                                if(inputBalances[i] != testNetBalance)
                                {
                                    writeLogLine(String.format("Net to gross to net mismatch: %d -> %d -> %d!", 
                                            inputBalances[i],testGrossBalance,testNetBalance));
                                    this_result=false;                                                       
                                }
                            }

                            String encoded=transfers.encodeToHex(inputCount,outputCount,40);
                            if(encoded != null)
                            {
                                if(!metadataHex.equals(encoded))
                                {
                                    writeLogLine("Encode transfer list mismatch: " + encoded + " should be " + metadataHex);
                                    this_result=false;
                                }                            
                            }
                            else
                            {
                                writeLogLine("Failed to encode transfer list " + metadataHex);
                                this_result=false;                   
                            }

                            if(!transfers.match(transfers,true))
                            {
                                writeLogLine("Failed to match transfer list to itself! " + metadataHex);                    
                                this_result=false;
                            }
                            
                        }
                        else
                        {
                            writeLogLine("Failed to decode transfers metadata " + metadataHex);
                            this_result=false;                    
                        }
                    }
                    else
                    {
                        writeLogLine("Failed to decode asset reference " + assetRefString);
                        this_result=false;                    
                    }
                }
                else
                {
                    writeLogLine("Failed to decode genesis " + genesisMetadataHex);
                    this_result=false;                    
                }
                                
                result &= this_result;
                if(!try_again)
                {
                    if(!this_result)
                    {
                        try_again=true;
                    }
                }
                else
                {
                    if(ExitOnFailure)
                    {
                        return result;
                    }
                    else
                    {
                        try_again=false;
                    }
                }
            }
		}
        
        return result;
    }
    
    private boolean performMessageHashTest(boolean ExitOnFailure)
    {
        boolean result=true;
        
        String line=getInputLine();
        if((line == null) || !line.equals("CoinSpark MessageHash Tests Input"))
        {
            writeLogLine("Different header line expected, got " + line);
            return false;
        }
        line=getInputLine();
        if((line == null) || (line.length()>0))
        {
            writeLogLine("Empty line expected, got " + line);
            return false;
        }
        
		writeOutputLine("CoinSpark MessageHash Tests Output");
		writeOutputLine("");
		
        String [] lines;
        
        while((lines=getInputLines(2)) != null)
        {
            boolean try_again=true;
            boolean this_result=true;
            String salt=lines[0];
            int countParts=Integer.valueOf(lines[1]);
            CoinSparkMessagePart [] contentParts=new CoinSparkMessagePart[countParts];            
            
            
            lines=getInputLines(3*countParts+1);
            
            if(lines != null)
            {                
                while(try_again)
                {
                    if(this_result)
                    {
                        try_again=false;
                    }
                    
                    for(int index=0;index<countParts;index++)
                    {
                        contentParts[index]=new CoinSparkMessagePart();
                        contentParts[index].mimeType=lines[index*3+0];
                        contentParts[index].fileName=lines[index*3+1];
                        contentParts[index].content=lines[index*3+2].getBytes();
                    }
                    
                    String hash=CoinSparkMessage.byteToHex(CoinSparkMessage.calcMessageHash(salt.getBytes(), contentParts));

                    if(hash != null)
                    {
                        writeOutputLine(hash);
                    }
                    else
                    {
                        writeLogLine("Cannot calcualte hash for " + salt);
                        this_result=false;                    
                    }

                    result &= this_result;
                    if(!try_again)
                    {
                        if(!this_result)
                        {
                            try_again=true;
                        }
                    }
                    else
                    {
                        if(ExitOnFailure)
                        {
                            return result;
                        }
                        else
                        {
                            try_again=false;
                        }
                    }
                }
            }
        }
        
        return result;
    }

    private boolean performAssetHashTest(boolean ExitOnFailure)
    {
        boolean result=true;
        
        String line=getInputLine();
        if((line == null) || !line.equals("CoinSpark AssetHash Tests Input"))
        {
            writeLogLine("Different header line expected, got " + line);
            return false;
        }
        line=getInputLine();
        if((line == null) || (line.length()>0))
        {
            writeLogLine("Empty line expected, got " + line);
            return false;
        }
        
		writeOutputLine("CoinSpark AssetHash Tests Output");
		writeOutputLine("");
		
        String [] lines;
        
        while((lines=getInputLines(10)) != null)
        {
            boolean try_again=true;
            boolean this_result=true;
            while(try_again)
            {
                if(this_result)
                {
                    try_again=false;
                }
                
                String name=lines[0];
                String issuer=lines[1];
                String description=lines[2];
                String units=lines[3];
                String issueDate=lines[4];
                String expiryDate=lines[5];
                double interestRate=Double.valueOf(lines[6]);
                double multiple=Double.valueOf(lines[7]);
                byte [] contract=lines[8].getBytes();

                String hash=CoinSparkGenesis.byteToHex(CoinSparkGenesis.calcAssetHash(name, issuer, description, units, issueDate, expiryDate, interestRate, multiple, contract));

                if(hash != null)
                {
                    writeOutputLine(hash);
                }
                else
                {
                    writeLogLine("Cannot calcualte hash for " + name);
                    this_result=false;                    
                }
                
                result &= this_result;
                if(!try_again)
                {
                    if(!this_result)
                    {
                        try_again=true;
                    }
                }
                else
                {
                    if(ExitOnFailure)
                    {
                        return result;
                    }
                    else
                    {
                        try_again=false;
                    }
                }
            }
		}
        
        return result;
    }


    private boolean performTest()
    {
        boolean result=true;
        
        if(testType == null)
        {
            System.out.print(String.format("Undefined test mode"));            
            return false;
        }
                
        System.out.print(String.format(testType.getText() + " test STARTED\n"));            
        
        inputBR=null;
        outputFW=null;
        logFW=null;
        
        File f = new File(inputFile);

        if (!f.exists()) 
        {
            System.out.print(String.format("Input file %s not found\n",inputFile));            
            result=false;
        }        
        else
        {
            f = new File(outputFile);
            if(f.exists())
            {
                f.delete();
            }
            f = new File(logFile);
            if(f.exists())
            {
                f.delete();
            }
            try {
                inputBR=new BufferedReader(new FileReader(inputFile));
            } catch (FileNotFoundException ex) {
                System.out.print(String.format("Cannot open input file %s\n",inputFile));            
                result=false;                                
            }
            try {
                outputFW = new FileWriter(outputFile);
            } catch (IOException ex) {
                System.out.print(String.format("Cannot open output file %s\n",outputFile));            
                result=false;                
            }
            try {
                logFW = new FileWriter(logFile);
            } catch (IOException ex) {
                System.out.print(String.format("Cannot open log file %s\n",logFile));            
                result=false;                
            }
        }
        
        switch(testType)
        {
            case ADDRESS:
                result &= performAddressTest(true);
                break;
            case GENESIS:
                result &= performGenesisTest(true);
                break;
            case HASH:
                result &= performAssetHashTest(true);
                break;
            case REFERENCE:
                result &= performAssetRefTest(true);
                break;
            case SCRIPT:
                result &= performScriptTest(true);
                break;
            case TRANSFER:
                result &= performTransferTest(true);
                break;            
            case MESSAGE:
                result &= performMessageHashTest(true);
                break;            
        }
        
        if(inputBR != null)
        {
            try {
                inputBR.close();
            } catch (IOException ex) {
                Logger.getLogger(CoinSparkTest.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        if(outputFW != null)
        {
            try {
                outputFW.close();
            } catch (IOException ex) {
                Logger.getLogger(CoinSparkTest.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        if(inputBR != null)
        {
            try {
                logFW.close();
            } catch (IOException ex) {
                Logger.getLogger(CoinSparkTest.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        
        if(result)
        {
            System.out.print(String.format(testType.getText() + " test COMPLETED SUCCESSFULLY\n"));            
        }
        else
        {
            System.out.print(String.format(testType.getText() + " test COMPLETED WITH ERRORS!!!\n"));                        
        }
        
        return result;
    }
    
    
    public static void main(String [] args)
    {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        boolean result=true;
        
        try
        {
            System.out.print(String.format("CoinSpark tests from existing input files\n"));
            System.out.print(String.format("-----------------------------------------------------------------------\n"));
            System.out.print(String.format("\n"));
            
            for (CoinSparkTestType tt : CoinSparkTestType.values()) 
            {
                System.out.print(tt.getLetter() + ": " + tt.getText() + "\n");
            }
            System.out.print(String.format("\n"));
            
            System.out.print("Choose a test suite(s) to run [all]: ");
            
            String testMode = br.readLine();
            if (testMode.length() == 0)
            {
                testMode="ARSHGTM";
            }            
            
            System.out.print(String.format("Directory name for tests: "));
            String testFilePath = br.readLine();            

            if (testFilePath.length() == 0)
            {
                testFilePath=".";
            }
            if (testFilePath.length() == 0)
            {
                System.out.print("Directory name not specified" + "\n");                    
                result=false;                
            }
            else
            {
                for(byte letter : testMode.getBytes())
                {
                    System.out.print(String.format("\n"));
                    CoinSparkTestType testType=CoinSparkTestType.fromLetter(new String(new byte[] {letter}));
                    if(testType != null)
                    {
                        CoinSparkTest test=new CoinSparkTest(testFilePath, testType);
                        result &= test.performTest();
                    }
                    else
                    {
                        System.out.print(String.format("Unsupported test mode " + new String(new byte[] {letter})) + "\n");                    
                        result=false;
                    }
                }
            }
            
        }       
        catch(IOException ioe){
            System.err.println(ioe.getMessage());
        }
        
        System.out.print(String.format("\n"));
        System.out.print(String.format("-----------------------------------------------------------------------\n"));
        System.out.print(String.format("\n"));
        if(result)
        {
            System.out.print(String.format("TESTS COMPLETED SUCCESSFULLY\n"));            
        }
        else
        {
            System.out.print(String.format("TESTS COMPLETED WITH ERRORS!!!\n"));                        
        }
        System.out.print(String.format("\n"));

    }
}
