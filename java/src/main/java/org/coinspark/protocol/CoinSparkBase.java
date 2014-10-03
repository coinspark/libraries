/*
 * CoinSpark 1.0 - Java library
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


package org.coinspark.protocol;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

/**
 * Base class implementing static utility functions and classes/utility functions used internally.
 */

public class CoinSparkBase {

//	Public constants
    protected static final byte COINSPARK_GENESIS_PREFIX  = 'g';
    protected static final byte COINSPARK_TRANSFERS_PREFIX = 't';
    protected static final byte COINSPARK_PAYMENTREF_PREFIX  = 'r';
    
    protected final static long COINSPARK_SATOSHI_QTY_MAX  = 2100000000000000L;

//	General public functions for managing CoinSpark metadata and bitcoin transaction output scripts    
    
    /**
     * Extracts OP_RETURN metadata (not necessarily CoinSpark data) from a bitcoin tx output script.
     * 
     * @param scriptPubKey Output script as hexadecimal.
     * @return byte [] | null Raw binary embedded metadata if found, null otherwise. 
     */
    
    public static byte [] scriptToMetadata(String scriptPubKey)
    {
        return scriptToMetadata(hexToByte(scriptPubKey));
    }

    /**
     * Extracts OP_RETURN metadata (not necessarily CoinSpark data) from a bitcoin tx output script.
     * 
     * @param scriptPubKey Output script as raw binary data.
     * @return byte [] | null Raw binary embedded metadata if found, null otherwise. 
     */
    
    public static byte [] scriptToMetadata(byte[] scriptPubKey)
    {
        if(scriptPubKey == null)
            return null;
        
        int scriptPubKeyLen = scriptPubKey.length;
        int metadataLength = scriptPubKeyLen-2;  // Skip the signature

        if ( (scriptPubKeyLen>2) && (scriptPubKey[0]==0x6a) &&
             (scriptPubKey[1]>0) && (scriptPubKey[1]<=75) && (scriptPubKey[1] == metadataLength))
        {
            return Arrays.copyOfRange(scriptPubKey, 2, scriptPubKeyLen);
        }

        return null;
    }
    
    /**
     * Extracts OP_RETURN metadata (not necessarily CoinSpark data) from a bitcoin tx output scripts.
     * 
     * @param scriptPubKeys Output scripts as hexadecimal.
     * @return metadata if found, null otherwise
     */
    
    public static byte [] scriptsToMetadata(String [] scriptPubKeys)
    {
        byte [][] raw=new byte[scriptPubKeys.length][];
        
        for(int i=0;i<scriptPubKeys.length;i++)
        {
            raw[i]=hexToByte(scriptPubKeys[i]);
        }
        
        return scriptsToMetadata(raw);
    }
    
    /**
     * Extracts OP_RETURN metadata (not necessarily CoinSpark data) from a bitcoin tx output scripts.
     * 
     * @param scriptPubKeys Output scripts as raw binary data.
     * @return metadata if found, null otherwise
     */
    
    public static byte [] scriptsToMetadata(byte[][] scriptPubKeys)
    {
        for (byte[] scriptPubKey : scriptPubKeys) {
            if (!scriptIsRegular(scriptPubKey)) {
                return scriptToMetadata(scriptPubKey);
            }
        }
        
        return null;
    }
    
    /**
     * Converts CoinSpark metadata (or other data) into an OP_RETURN bitcoin tx output script.
     * 
     * @param metadata metadata  Raw binary metadata to be converted.
     * @return string | null The OP_RETURN bitcoin tx output script as hexadecimal, null if we failed.
     */
    
    public static String metadataToScriptHex(byte [] metadata)
    {
        return byteToHex(metadataToScript(metadata));
    }

    /**
     * Converts CoinSpark metadata (or other data) into an OP_RETURN bitcoin tx output script.
     * 
     * @param metadata metadata  Raw binary metadata to be converted.
     * @return byte [] | null The OP_RETURN bitcoin tx output script as raw binary, null if we failed.
     */
    
    public static byte[] metadataToScript(byte [] metadata)
    {
        byte [] scriptPubKey;
        if ( (metadata.length <= 75))// && (scriptPubKeyMaxLen>=scriptLength) ) {
        {
            int scriptRawLen = metadata.length+2;

            scriptPubKey=new byte[scriptRawLen];
            
            scriptPubKey[0]=0x6a;
            scriptPubKey[1] = (byte)metadata.length;
            
            System.arraycopy(metadata, 0, scriptPubKey, 2, metadata.length);
            return scriptPubKey;
        }

        return null;
    }

    /**
	 * Calculates the maximum length of CoinSpark metadata that can be added to some existing CoinSpark metadata
	 * to fit into a specified number of bytes.
	 * 
	 * The calculation is not simply metadataMaxLen-metadata.length because some space is saved when combining pieces of CoinSpark metadata together.
	 *
     * @param metadata The existing CoinSpark metadata in raw binary form, which can itself already be
	 *    a combination of more than one CoinSpark metadata element.
     * @param metadataMaxLen The total number of bytes available for the combined metadata.
     * @return integer The number of bytes which are available for the new piece of metadata to be added.
     */
    
    public static int metadataMaxAppendLen(byte [] metadata, int metadataMaxLen)
    {
        return Math.max(metadataMaxLen - (metadata.length + 1 - COINSPARK_METADATA_IDENTIFIER_LEN), 0);
    }
    
    /**
     * Appends one piece of CoinSpark metadata to another.
     * 
     * @param metadata The existing CoinSpark metadata in raw binary form (or null), which can itself already be
	 *    a combination of more than one CoinSpark metadata element. 
     * @param metadataMaxLen The total number of bytes available for the combined metadata.
     * @param appendMetadata The new CoinSpark metadata to be appended, in raw binary form.
     * @return byte [] | null The combined CoinSpark metadata as raw binary, or null if we failed.
     */    
    
    public static byte [] metadataAppend(byte [] metadata, int metadataMaxLen, byte [] appendMetadata)
    {
        if(metadata == null)                                                    // metadata == null case
        {
            if(metadataMaxLen < appendMetadata.length)                          // check append metdata is short enough
                return null;
            
            return appendMetadata;
        }
        
        CoinSparkBase.CoinSparkBuffer oldBuffer=new CoinSparkBase().new CoinSparkBuffer(metadata);

        if(!oldBuffer.locateRange((byte)0))                                     // check we can find last metadata
            return null;
        
        if(appendMetadata.length < COINSPARK_METADATA_IDENTIFIER_LEN + 1)       // check there is enough to check the prefix
            return null;
        
        if (memcmp(COINSPARK_METADATA_IDENTIFIER.getBytes(), appendMetadata, COINSPARK_METADATA_IDENTIFIER_LEN) != 0) // check the prefix
            return null;
        
        int needLength=metadata.length+appendMetadata.length-COINSPARK_METADATA_IDENTIFIER_LEN+1;
        if(metadataMaxLen < needLength)                                         // check there is enough space
            return null;
        
        
        int lastMetadataLen=oldBuffer.availableForRead()+1;                     // include prefix
        int lastMetaDataPos=oldBuffer.offsetRead-1;
        
        CoinSparkBase.CoinSparkBuffer newBuffer=new CoinSparkBase().new CoinSparkBuffer(); 
        newBuffer.writeBytes(metadata, lastMetaDataPos);                        // Data before last metadata
        newBuffer.writeByte((byte)lastMetadataLen);                             // Length prefix for last metadata
        newBuffer.writeByte(metadata[lastMetaDataPos]);                         // Last metadata prefix
        newBuffer.writeBytes(oldBuffer.readBytes(oldBuffer.availableForRead()));// Last metadata without identifier and prefix
        
        newBuffer.writeBytes(Arrays.copyOfRange(appendMetadata, COINSPARK_METADATA_IDENTIFIER_LEN, appendMetadata.length));// Appended metadata
        
        return newBuffer.toBytes();
    }
    
    /**
     * Tests whether a bitcoin tx output script is 'regular', i.e. not an OP_RETURN script.
     * 
	 * This function will declare empty scripts or invalid hex scripts as 'regular' as well, since they are not OP_RETURNs.
	 * Use this to build $outputsRegular arrays which are used by various other functions.
     * 
     * @param scriptPubKey Output script as hexadecimal.
     * @return true if the script is 'regular', false if it is an OP_RETURN script.
     */
    
    public static boolean scriptIsRegular(String scriptPubKey)
    {
        return scriptIsRegular(hexToByte(scriptPubKey));
    }
    
    /**
     * Tests whether a bitcoin tx output script is 'regular', i.e. not an OP_RETURN script.
     * 
	 * This function will declare empty scripts or invalid hex scripts as 'regular' as well, since they are not OP_RETURNs.
	 * Use this to build $outputsRegular arrays which are used by various other functions.
     * 
     * @param scriptPubKey Output script as raw binary data.
     * @return true if the script is 'regular', false if it is an OP_RETURN script.
     */

    public static boolean scriptIsRegular(byte[] scriptPubKey)
    {
        return (scriptPubKey.length < 1) || (scriptPubKey[0] != 0x6a);
    }


    
//	Utitlity functions/classes used internally in CoinSpark Library    
    
    private static final long COINSPARK_FEE_BASIS_MAX_SATOSHIS  = 1000;
    
    protected static long getMinFeeBasis(long[] outputsSatoshis, boolean[] outputsRegular)
    {
        if(outputsSatoshis.length != outputsRegular.length)
        {
            return COINSPARK_SATOSHI_QTY_MAX;
        }
        int countOutputs=outputsRegular.length;
        long smallestOutputSatoshis  = COINSPARK_SATOSHI_QTY_MAX;

        for (int outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            if (outputsRegular[outputIndex])
                if (smallestOutputSatoshis > outputsSatoshis[outputIndex])
                    smallestOutputSatoshis = outputsSatoshis[outputIndex];
        }

        if (smallestOutputSatoshis > COINSPARK_FEE_BASIS_MAX_SATOSHIS)
            smallestOutputSatoshis = COINSPARK_FEE_BASIS_MAX_SATOSHIS;
        return smallestOutputSatoshis;
    }
    
    protected final static String COINSPARK_METADATA_IDENTIFIER    = "SPK";
    
    private static final byte[] hexCharMap = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    private static final int[] base58Minus49ToInteger = { // 74 elements
            0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1, -1,
            9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1, 22,
            23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1, -1,
            33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46, 47,
            48, 49, 50, 51, 52, 53, 54, 55, 56, 57    };
    private final static int COINSPARK_METADATA_IDENTIFIER_LEN   = 3;
    private final static int COINSPARK_LENGTH_PREFIX_MAX         = 96;

    /**
     * Returns SHA256 hash of the input raw data
     * @param input input raw data
     * @param inputLen actual size of the data to hash (raw data may be longer)
     * C function: void CoinSparkCalcSHA256Hash(const unsigned char* input, const size_t inputLen, unsigned char hash[32]);
     * @return SHA-256 hash 
     */
    
    protected static byte [] coinSparkCalcSHA256Hash(byte[] input, int inputLen) 
    {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            return digest.digest(Arrays.copyOf(input, inputLen));
        } catch (NoSuchAlgorithmException ex) {
            return null;
        }
    }
    
    
    protected static int base58ToInteger(byte base58Character)                  // returns -1 if invalid
    {
        if ( (base58Character<49) || (base58Character>122) )
            return -1;

        return base58Minus49ToInteger[base58Character-49];
    }

    protected static String byteToHex(byte [] raw)
    {
        if(raw==null)
            return null;
        
        byte [] hexBytes=new byte [raw.length*2];
        
        for(int i=0;i<raw.length;i++)
        {
            short value=(short)(raw[i] & 0xFF);
            hexBytes[2*i+0]=hexCharMap[(byte)(value>>4)];
            hexBytes[2*i+1]=hexCharMap[(byte)(value&15)];
        }
        
        return new String(hexBytes);
    }
    
    protected static byte[] hexToByte(String str)
    {
        if (str == null)
            return null;

        byte[] bytes = new byte[str.length() / 2];
        for (int i = 0; i < bytes.length; i++)
        {
            bytes[i] = (byte) Integer
                    .parseInt(str.substring(2 * i, 2 * i + 2), 16);
        }
        return bytes;
    }
    
    protected static byte[] unsignedToSmallEndianBytes(long value, int bytes)
    {
        if (bytes <= 0)
            return null;

        byte [] raw=new byte[bytes];
        
        for (int index=0; index<bytes; index++)
        {
            raw[index]=(byte)(value & 0xFF);
            value /= 256;
        }        
        
        return raw;
    }
    
    protected static String unsignedToSmallEndianHex(long value, int bytes)
    {
        return byteToHex(unsignedToSmallEndianBytes(value,bytes));
    }

    protected static Long SmallEndianBytesToUnsigned(byte[] dataBuffer, int offset,  int bytes)
    {
        if (bytes < 0 || offset < 0 || bytes+offset >dataBuffer.length)
            return null;

        long value = 0;

        for (int curbyte=bytes-1; curbyte>=0; curbyte--)
        {
            value *= 256;
            int ttt = dataBuffer[curbyte + offset];
            if (ttt < 0) ttt &= 0xFF;     
            value += ttt;
        }

        return  value & 0x7FFFFFFFFFFFFFFFL;
    }
    
    protected static int memcmp(byte b1[], byte b2[], int sz){
        for(int i = 0; i < sz; i++){
            if(b1[i] != b2[i]){
                if((b1[i] >= 0 && b2[i] >= 0)||(b1[i] < 0 && b2[i] < 0))
                    return b1[i] - b2[i];
                if(b1[i] < 0 && b2[i] >= 0)
                    return 1;
                if(b2[i] < 0 && b1[i] >=0)
                    return -1;
            }
        }
        return 0;
    }
    
    
    protected static int getLastRegularOutput(boolean[] outputsRegular)
    {
        int countOutputs=outputsRegular.length;
        int outputIndex;

        for (outputIndex=countOutputs-1; outputIndex>=0; outputIndex--) 
        {
            if (outputsRegular[outputIndex])
                return outputIndex;
        }

        return countOutputs; // indicates no regular ones were found
    }

    
    protected class CoinSparkBuffer{
        
        private final static int BUFFER_ALLOC_LENGTH = 40;
        
        private boolean resizable;
        private byte [] raw;
        private int offsetRead;
        private int offsetWrite;
        private int sizeRead;
        

        public CoinSparkBuffer(int size)
        {
            raw=new byte[size];
            offsetRead=0;
            offsetWrite=0;
            sizeRead=offsetWrite;
            resizable=false;
        }
        
        CoinSparkBuffer()
        {
            this(BUFFER_ALLOC_LENGTH);
            resizable=true;
        }

        CoinSparkBuffer(String Source,boolean IsHex)
        {
            if(IsHex)
            {
                raw=hexToByte(Source);
            }
            else
            {
                raw=Source.getBytes();
            }
            offsetRead=0;
            offsetWrite=raw.length;
            sizeRead=offsetWrite;
            resizable=false;
        }
        
        CoinSparkBuffer(byte [] Source)
        {
            raw=Source;
            offsetRead=0;
            offsetWrite=raw.length;
            sizeRead=offsetWrite;
            resizable=false;
        }
        
        protected int length()
        {
            return offsetWrite;
        }
        
        protected int availableForRead()
        {
            return sizeRead-offsetRead;
        }
        
        protected void resetReadOffset()
        {
            offsetRead=0;           
            sizeRead=offsetWrite;            
        }
        
        protected void resetWriteOffset()
        {
            offsetWrite=0;            
        }
        
        protected boolean realloc(int bytes)
        {
            int size=raw.length;
            while(size<offsetWrite+bytes)
            {
                size+=BUFFER_ALLOC_LENGTH;
            }
            if(size>raw.length)
            {
                if(!resizable)
                {
                    return false;
                }
                byte [] newRaw=new byte[size];
                System.arraycopy(raw, 0, newRaw, 0, offsetWrite);
                raw=newRaw;
            }
            return true;
        }
        
        protected byte [] toBytes()
        {
            if(offsetWrite==0)
            {
                return null;
            }
            
            return Arrays.copyOf(raw, offsetWrite);
        }
        
        protected String toAscii()
        {
            return new String(toBytes());
        }
        
        protected String toHex()
        {
            return byteToHex(toBytes());
        }
        
        protected boolean writeByte(byte b) 
        {
            if(!realloc(1))
                return false;
                
            raw[offsetWrite]=b;
            offsetWrite++;
            return true;
        }

        protected boolean writeInt(int value,int size) 
        {
            return writeLong(value, size);
        }

        protected boolean writeLong(long value,int size) 
        {
            return writeBytes(unsignedToSmallEndianBytes(value, size));
        }
        
        protected boolean writeBytes(byte [] b) 
        {
            if(b==null)
                return true;
            
            return writeBytes(b,b.length);
        }
        
        protected boolean writeBytes(byte [] b,int size) 
        {
            if(size<=0)
                return true;
            
            if(!realloc(size))
                return false;
            
            System.arraycopy(b, 0, raw, offsetWrite, size);
            
            offsetWrite+=size;
            return true;
        }
        
        protected boolean writeString(String s)
        {
            return writeBytes(s.getBytes());
        }
                
        protected Byte readByte()
        {
            if(offsetRead>=sizeRead)
             return null;
            
            offsetRead++;
            return raw[offsetRead-1];
        }
     
        protected Long readLong(int size)
        {
            if(offsetRead+size>sizeRead)
             return null;
            
            offsetRead+=size;
            
            return SmallEndianBytesToUnsigned(raw, offsetRead-size, size);
        }

        protected Integer readInt(int size)
        {          
            if(size>4)
                return null;
            
            if(offsetRead+size>sizeRead)
             return null;
         
            return readLong(size).intValue();
        }
        
        protected byte [] readBytes(int size)
        {
            if(offsetRead+size>sizeRead)
             return null;

            offsetRead+=size;
            
            return Arrays.copyOfRange(raw, offsetRead-size, offsetRead);
        }
        
        
        protected boolean canRead(int size)
        {
            if(offsetRead+size>sizeRead)
             return false;
            return true;
        }
        
        protected boolean locateRange(byte desiredPrefix)
        {
            offsetRead=0;
            if(!canRead(COINSPARK_METADATA_IDENTIFIER_LEN+1))
                return false;

            if (memcmp(COINSPARK_METADATA_IDENTIFIER.getBytes(), raw, COINSPARK_METADATA_IDENTIFIER_LEN) != 0) // check it starts 'SPK'
                return false;

            offsetRead+=COINSPARK_METADATA_IDENTIFIER_LEN;                      // skip past 'SPK'

            while (offsetRead < offsetWrite) 
            {
                byte foundPrefix = readByte();                                  // read the next prefix

                if (desiredPrefix != 0 ? (foundPrefix==desiredPrefix) : (foundPrefix > COINSPARK_LENGTH_PREFIX_MAX))
                {
                    // it's our data from here to the end (if desiredPrefix is 0, it matches the last one whichever it is)
                    sizeRead=offsetWrite;
                    return true;
                }

                if (foundPrefix>COINSPARK_LENGTH_PREFIX_MAX)                    // it's some other type of data from here to end
                    return false;

                // if we get here it means we found a length byte

                if (offsetRead+foundPrefix > offsetWrite)                       // something went wrong - length indicated is longer than that available
                    return false;

                if (offsetRead >= offsetWrite)                                  // something went wrong - that was the end of the input data
                    return false;

                if (raw[offsetRead] == desiredPrefix) 
                {                                                               // it's the length of our part
                    offsetRead++;
                    sizeRead=offsetRead+foundPrefix-1;
                    return true;
                }
                else
                {
                    offsetRead+=foundPrefix;
                }
            }

            return false;
        }
        
    }

}
