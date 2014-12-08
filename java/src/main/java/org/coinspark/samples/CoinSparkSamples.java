/*
 * CoinSpark 1.0 - Java sample code
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



package org.coinspark.samples;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.coinspark.protocol.CoinSparkAddress;
import org.coinspark.protocol.CoinSparkAssetRef;
import org.coinspark.protocol.CoinSparkBase;
import org.coinspark.protocol.CoinSparkGenesis;
import org.coinspark.protocol.CoinSparkIORange;
import org.coinspark.protocol.CoinSparkMessage;
import org.coinspark.protocol.CoinSparkPaymentRef;
import org.coinspark.protocol.CoinSparkTransfer;
import org.coinspark.protocol.CoinSparkTransferList;

/**
 *
 * @author mike
 */
public class CoinSparkSamples {
    
    public static void CreateCoinSparkAddress()
    {        
        System.out.println("\nCreating a CoinSpark address...\n");

        CoinSparkAddress address=new CoinSparkAddress();

        address.setBitcoinAddress("149wHUMa41Xm2jnZtqgRx94uGbZD9kPXnS");
        address.setAddressFlags(CoinSparkAddress.COINSPARK_ADDRESS_FLAG_ASSETS | CoinSparkAddress.COINSPARK_ADDRESS_FLAG_PAYMENT_REFS);
        address.setPaymentRef(new CoinSparkPaymentRef(0)); // or any unsigned 52-bit integer up to CoinSparkPaymentRef.COINSPARK_PAYMENT_REF_MAX

        String addressString=address.encode();

        if (addressString!=null)
            System.out.println("CoinSpark address: "+addressString);
        else
            System.out.println("CoinSpark address encode failed!");
    }
    
    public static void DecodeCoinSparkAddress()
    {
        System.out.println("\nDecoding a CoinSpark address...\n");

        CoinSparkAddress address=new CoinSparkAddress();

        if (address.decode("s6GUHy69HWkwFqzFhJCY49seL8EFv")) {
            System.out.println("Bitcoin address: "+address.getBitcoinAddress());
            System.out.println("Address flags: "+address.getAddressFlags());
            System.out.println("Payment reference: "+address.getPaymentRef().getRef());

            System.out.print(address.toString());

        } else
            System.out.println("CoinSpark address decode failed!");
    }
    
    public static void ProcessTransaction(byte[][] scriptPubKeys, int countInputs)
    {
        System.out.println("\nExtracting CoinSpark metadata from a transaction...\n");

        // scriptPubKeys is an array containing each output script of a transaction as raw binary data.       
        // The transaction has scriptPubKeys.length outputs and countInputs inputs.

        byte[] metadata=CoinSparkBase.scriptsToMetadata(scriptPubKeys);

        if (metadata!=null) {
            CoinSparkGenesis genesis=new CoinSparkGenesis();                                
            if (genesis.decode(metadata))
                System.out.print(genesis.toString());
                    
            CoinSparkTransferList transferList=new CoinSparkTransferList();
            if (transferList.decode(metadata, countInputs, scriptPubKeys.length))
                System.out.print(transferList.toString());                    
    
            CoinSparkPaymentRef paymentRef=new CoinSparkPaymentRef();
            if (paymentRef.decode(metadata))
                System.out.print(paymentRef.toString());
        }
    }
    
    public static void ProcessTransaction(String[] scriptPubKeys, int countInputs)
    {
        System.out.println("\nExtracting CoinSpark metadata from a transaction...\n");

        // scriptPubKeys is an array containing each output script of a transaction as a hex string
        // or raw binary (commented above). The transaction has scriptPubKeys.length outputs and
        // countInputs inputs.

        byte[] metadata=CoinSparkBase.scriptsToMetadata(scriptPubKeys);

        if (metadata!=null) {
            CoinSparkGenesis genesis=new CoinSparkGenesis();                                
            if (genesis.decode(metadata))
                System.out.print(genesis.toString());
                    
            CoinSparkTransferList transferList=new CoinSparkTransferList();
            if (transferList.decode(metadata, countInputs, scriptPubKeys.length))
                System.out.print(transferList.toString());                    
    
            CoinSparkPaymentRef paymentRef=new CoinSparkPaymentRef();
            if (paymentRef.decode(metadata))
                System.out.print(paymentRef.toString());
        }
    }
    
    public static byte[] EncodeMetaData(byte[] metadata)
    {
        System.out.println("\nEncoding CoinSpark metadata in a script...\n");

        // first get metadata from the encode() method of a CoinSparkGenesis, CoinSparkTransferList
        // or CoinSparkPaymentRef object, or the CoinSparkBase.metadataAppend() method.

        byte[] scriptPubKey=null;
        
        if (metadata!=null) {
            scriptPubKey=CoinSparkBase.metadataToScript(metadata);

            if (scriptPubKey!=null)
                ; // now embed the raw bytes in $scriptPubKey directly in a transaction output
            else
                ; // handle the error
    
        } else
            ; // handle the error

        return scriptPubKey;
    }
    
    public static String EncodeMetaDataToHex(byte [] metadata)
    {
        System.out.println("\nEncoding CoinSpark metadata in a script...\n");

        // first get metadata from the encode() method of a CoinSparkGenesis, CoinSparkTransferList
        // or CoinSparkPaymentRef object, or the CoinSparkBase.metadataAppend() method.

        String scriptPubKey=null;
        
        if (metadata!=null) {
            scriptPubKey=CoinSparkBase.metadataToScriptHex(metadata);

            if (scriptPubKey!=null)
                System.out.println("Script: "+scriptPubKey);
            else
                System.out.println("Metadata encode failed!");

        } else
            ; // handle the error

        return scriptPubKey;
    }
    
    public static byte[] CoinSparkPaymentRefTransfersEncode(CoinSparkPaymentRef paymentRef,
        CoinSparkTransferList transferList, int countInputs, int countOutputs, int metadataMaxLen)
    {
        byte[] metadata=paymentRef.encode(metadataMaxLen);
        if (metadata==null)
            return null;

        int appendMetadataMaxLen=CoinSparkBase.metadataMaxAppendLen(metadata, metadataMaxLen);
            // this is not simply metadataMaxLen-metadata.length since combining saves space

        byte[] appendMetaData=transferList.encode(countInputs, countOutputs, appendMetadataMaxLen);
        if (appendMetaData==null)
            return null;
   
        return CoinSparkBase.metadataAppend(metadata, metadataMaxLen, appendMetaData);
    }
    
    public static CoinSparkGenesis CreateGenesis()
    {
        System.out.println("\nCreating and encoding genesis metadata...\n");

        CoinSparkGenesis genesis=new CoinSparkGenesis();

        genesis.setQty(1234567, 1); // 1234567 units rounded up     
        long actualQty=genesis.getQty(); // can check final quantity assigned

        genesis.setChargeFlat(4321, 0); // 4321 units rounded to nearest
        long actualChargeFlat=genesis.getChargeFlat(); // can check final flat charge assigned

        genesis.setChargeBasisPoints((short)10); // additional 0.1% per payment

        genesis.setUseHttps(false);
        genesis.setDomainName("www.example.com");
        genesis.setUsePrefix(true);
        genesis.setPagePath("usd-1");

        int assetHashLen=genesis.calcHashLen(40); // 40 byte limit for OP_RETURN
        genesis.setAssetHashLen(assetHashLen);
        byte[] assetHash = new byte [assetHashLen];        
        Random rnd=new Random();        
        rnd.nextBytes(assetHash); // random hash in example    
        genesis.setAssetHash(assetHash);

        byte[] metadata=genesis.encode(40); // 40 byte limit for OP_RETURNs

        if (metadata!=null)
            ; // use CoinSparkBase.metadataToScript() to embed metadata in an output script
        else
            ; // handle error

        return genesis;
    }

    public static CoinSparkMessage CreateMessage()
    {
        System.out.println("\nCreating and encoding message metadata...\n");

        CoinSparkMessage message=new CoinSparkMessage();
 
        message.setUseHttps(true);
        message.setServerHost("123.45.67.89");
        message.setUsePrefix(false);
        message.setServerPath("msg");
        message.setIsPublic(false);
        message.addOutputs(new CoinSparkIORange(0, 2)); // message is for outputs 0 and 1
 
        int countOutputs=3; // 3 outputs for this transaction
        int hashLen=message.calcHashLen(countOutputs, 40); // 40 byte limit for OP_RETURN
        message.setHashLen(hashLen);
        byte[] hash = new byte [hashLen];       
        Random rnd=new Random();       
        rnd.nextBytes(hash); // random hash in example   
        message.setHash(hash);

        byte[] metadata=message.encode(countOutputs, 40); // 40 byte limit for OP_RETURNs

        if (metadata!=null)
            ; // use CoinSparkBase.metadataToScript() to embed metadata in an output script
        else
            ; // handle error
        
        return message;
    }
    
    public static CoinSparkTransferList CreateTransferList()
    {
        System.out.println("\nCreating and encoding transfer metadata...\n");

        int countInputs=3;
        int countOutputs=5;
        CoinSparkTransferList transferList=new CoinSparkTransferList();

        CoinSparkTransfer transfer=new CoinSparkTransfer();
        CoinSparkAssetRef assetRef=new CoinSparkAssetRef();
        assetRef.decode("456789-65432-23456");
        transfer.setAssetRef(assetRef);
        transfer.setInputs(new CoinSparkIORange(0, 2)); // transfer from inputs 0 and 1
        transfer.setOutputs(new CoinSparkIORange(0, 1)); // transfer to outputs 0 only
        transfer.setQtyPerOutput(123);
        transferList.setTransfer(0, transfer);

        transfer=new CoinSparkTransfer();
        transfer.setAssetRef(transferList.getTransfer(0).getAssetRef()); // second transfer is for same asset type
        transfer.setInputs(new CoinSparkIORange(2, 1)); // transfer from input 2 only
        transfer.setOutputs(new CoinSparkIORange(1, 3)); // transfer to outputs 1, 2 and 3
        transfer.setQtyPerOutput(456);    
        transferList.setTransfer(1, transfer);

        byte[] metadata=transferList.encode(countInputs, countOutputs, 40); // 40 byte limit for OP_RETURNs

        if (metadata!=null)
            ; // use CoinSparkBase.metadataToScript() to embed metadata in an output script
        else
            ; // handle error

        return transferList;
    }
    
    public static long[] CalcTransactionOutputBalances(
        CoinSparkAssetRef assetRef, CoinSparkGenesis genesis, 
        CoinSparkTransferList transferList,
        long minFeeSatoshis, long feeSatoshis, 
        long[] inputBalances, boolean[] outputsRegular)
    {
        // assetRef is a CoinSparkAssetRef object describing the CoinSpark asset type of interest.
        // genesis is a CoinSparkGenesis object describing the same asset type.
        // transferList is a CoinSparkTransferList object from the decoded transfer metadata -
        // If there is no transfer metadata, you can pass in a new CoinSparkTransferList().
        // minFeeSatoshis is the minimum transaction fee required to make those transfers valid.
        // feeSatoshis is the quantity of bitcoin satoshis in this transaction's fee.
        // inputBalances is an array of the asset's balances in the transaction's inputs.
        // outputsRegular is an array indicating which outputs are regular, via CoinSparkBase.scriptIsRegular().
   
        // Returns the balances of that asset in each output

        long[] outputBalances;

        if (feeSatoshis>=minFeeSatoshis)
            outputBalances=transferList.apply(assetRef, genesis, inputBalances, outputsRegular);
        else
            outputBalances=transferList.applyNone(assetRef, genesis, inputBalances, outputsRegular);

        return outputBalances;
    }
    
    public static CoinSparkAssetRef CreateAssetRef()
    {
        System.out.println("\nFormatting an asset reference for users...\n");

        CoinSparkAssetRef assetRef=new CoinSparkAssetRef();

        assetRef.setBlockNum(456789);
        assetRef.setTxOffset(65432);
        assetRef.setTxIDPrefix(new byte[] {(byte)0xa0, (byte)0x5b});

        System.out.println("Asset reference: "+assetRef.encode());

        return assetRef;
    }
    
    public static CoinSparkAssetRef readAssetRef()
    {
        System.out.println("\nReading a user-provided asset reference...\n");

        CoinSparkAssetRef assetRef=new CoinSparkAssetRef();

        if (assetRef.decode("456789-65432-23456")) {
            System.out.println("Block number: "+assetRef.getBlockNum());
            System.out.println("Byte offset: "+assetRef.getTxOffset());
            System.out.println("TxID prefix: "+String.format("%02X%02X", assetRef.getTxIDPrefix()[0], assetRef.getTxIDPrefix()[1]));
    
            System.out.print(assetRef.toString());            

        } else
            System.out.println("Asset reference could not be read!");                        

        return assetRef;
    }

    private static String file_get_contents (String Address)
    {    
        URL url;
        try {
            url = new URL(Address);
        } catch (MalformedURLException ex) {
            Logger.getLogger(CoinSparkSamples.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
        
        URLConnection connection;
        try {        
            connection = url.openConnection();
        } catch (IOException ex) {
            Logger.getLogger(CoinSparkSamples.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
        
        InputStream input;
        try {
            input = connection.getInputStream();
        } catch (IOException ex) {
            Logger.getLogger(CoinSparkSamples.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
        byte[] buffer = new byte[4096];
        int n = - 1;

        StringBuilder sb = new StringBuilder();        
        try {
            while ( (n = input.read(buffer)) != -1)
            {
                if (n > 0)
                {
                    sb.append(new String(buffer, 0, n, "UTF-8"));
                }
            }
        } catch (IOException ex) {
            Logger.getLogger(CoinSparkSamples.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
        
        return sb.toString();
    }
    
    public static byte[] CalculateAssetHash()
    {
        String name="Credit at John Doe's";
        String issuer="John Doe's Restaurant Chain";
        String description="Can be used for any purchase at John Doe's, excluding breakfasts.";
        String units="1 US Dollar";
        String issueDate="2014-03-01";
        String expiryDate="2024-02-29";
        double interestRate=0.0;
        double multiple=0.01;
        String contractURL="http://www.john-doe-dining.com/bitcoin-credits-contract.pdf";

        byte[] contractContent;
        String contractContentString=file_get_contents(contractURL);
        if(contractContentString != null)
        {
            contractContent=contractContentString.getBytes();
        }
        else
        {
            ; // handle error
            return null;
        }
            // assume we have some function like this that we can use

        byte[] assetHash=null;

        if (contractContent!=null)
            assetHash=CoinSparkGenesis.calcAssetHash(name, issuer, description, units, issueDate,
                expiryDate, interestRate, multiple, contractContent);
        else
            ; // handle error

        return assetHash;        
    }
    
    public static CoinSparkPaymentRef CreatePaymentRef()
    {
        System.out.println("\nCreating and encoding payment reference metadata...\n");

        CoinSparkPaymentRef paymentRef=new CoinSparkPaymentRef();
        paymentRef.randomize(); // randomizes the payment reference
        byte[] metadata = paymentRef.encode(40); // assume 40 byte limit for OP_RETURNs

        if (metadata!=null)
            ; // use CoinSparkBase.metadataToScript() to embed metadata in an output script
        else
            ; // handle error

        return paymentRef;
    }

    // public static boolean CheckGenesisFee(CoinSparkGenesis genesis, long feeSatoshis,
    //     long[] outputsSatoshis, byte[][] scriptPubKeys)

    public static boolean CheckGenesisFee(CoinSparkGenesis genesis, long feeSatoshis,
        long[] outputsSatoshis, String[] scriptPubKeys)
    {
        // genesis is a CoinSparkGenesis object, already extracted from the transaction.
        // feeSatoshis is the quantity of bitcoin satoshis in the transaction fee.
        // outputsSatoshis is an array of bitcoin satoshi quantities in each output of the transaction.
        // scriptPubKeys is an array containing each output script of a transaction as a hex string
        // or raw binary (commented above), so that the transaction has scriptPubKeys.length outputs.

        int countOutputs=scriptPubKeys.length;
        boolean[] outputsRegular=new boolean[countOutputs];

        for (int output=0; output<countOutputs; output++)
            outputsRegular[output]=CoinSparkBase.scriptIsRegular(scriptPubKeys[output]);

        return feeSatoshis>=genesis.calcMinFee(outputsSatoshis, outputsRegular);
    }

    // public static boolean CheckTransfersFee(CoinSparkTransferList transferList, long feeSatoshis,
    //     long[] outputsSatoshis, byte[][] scriptPubKeys, int countInputs)

    public static boolean CheckTransfersFee(CoinSparkTransferList transferList, long feeSatoshis,
        long[] outputsSatoshis, String[] scriptPubKeys, int countInputs)
    {
        // transferList is a CoinSparkTransferList object, already extracted from the transaction.
        // feeSatoshis is the quantity of bitcoin satoshis in the transaction fee.
        // outputsSatoshis is an array of bitcoin satoshi quantities in each output of the transaction.
        // scriptPubKeys is an array containing each output script of a transaction as a hex string
        // or raw binary (commented above), so that the transaction has scriptPubKeys.length outputs
        // and countInputs inputs.

        int countOutputs=scriptPubKeys.length;
        boolean[] outputsRegular=new boolean[countOutputs];

        for (int output=0; output<countOutputs; output++)
            outputsRegular[output]=CoinSparkBase.scriptIsRegular(scriptPubKeys[output]);

        return feeSatoshis>=transferList.calcMinFee(countInputs, outputsSatoshis, outputsRegular);
    }
    
    public static void main(String [] args)
    {
        CreateCoinSparkAddress();

        DecodeCoinSparkAddress();

        ProcessTransaction(new String [] {"6A2853504B6750A4AE00F454956DF4C7D6DE7BF8192486006A4ADF65B048BF847FE26D70588E9FA828D5"},15856);

        ProcessTransaction(new String [] {"abc","6A2053504B743F282321E438188C4B381807227C10812B47920642B32E12417D8279","def"},59364);
        
        ProcessTransaction(new String [] {"6A2553504B0872876AAE4C1CC00A747A3E6F1BC14CD7752DA0D507BD05ED903A1C8407CCE38087"},1925);

        byte [] metadataTransfers = CoinSparkBase.scriptToMetadata("6A2053504B743F282321E438188C4B381807227C10812B47920642B32E12417D8279");
        EncodeMetaDataToHex(metadataTransfers);

        CoinSparkGenesis genesis=CreateGenesis();

        CoinSparkMessage message=CreateMessage();
        
        CoinSparkTransferList transferList=CreateTransferList();

        CoinSparkPaymentRef paymentRef=CreatePaymentRef();

        byte [] metadata = CoinSparkPaymentRefTransfersEncode(paymentRef,transferList,3, 5, 40);
        
        ProcessTransaction(new byte[][] {EncodeMetaData(metadata),new byte[0],new byte[0],new byte[0],new byte[0]},3);

        CoinSparkAssetRef assetRef=CreateAssetRef();

        readAssetRef();

        CalculateAssetHash();
    }
    
}
