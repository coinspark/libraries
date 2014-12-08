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

import java.util.Arrays;

/**
 * CoinSparkMessage class for managing CoinSpark messages
 */


public class CoinSparkMessage extends CoinSparkBase{
    
// Public functions    

    /**
     * Returns Server Host
     * 
     * @return Server Host
     */

    public String getServerHost() {
        return serverHost;
    }

    /**
     * Sets DomainName
     * 
     * @param ServerHost to set
     */
    
    public void setServerHost(String ServerHost) {
        serverHost = ServerHost;
    }

    /**
     * Returns Server Path
     * 
     * @return Server Path
     */

    public String getServerPath() {
        return serverPath;
    }

    /**
     * Sets Server Path
     * 
     * @param Server to set
     */
    
    public void setServerPath(String Server) {
        serverPath = Server;
    }

    /**
     * Returns Use https flag
     * 
     * @return Use https flag
     */

    public boolean getUseHttps() {
        return useHttps;
    }

    /**
     * Sets Use https flag
     * 
     * @param UseHttps to set
     */
    
    public void setUseHttps(boolean UseHttps) {
        useHttps = UseHttps;
    }

    /**
     * Returns Use path prefix flag
     * 
     * @return Use path prefix flag
     */

    public boolean getUsePrefix() {
        return usePrefix;
    }

    /**
     * Sets Use path prefix flag
     * 
     * @param UsePrefix flag to set
     */
    
    public void setUsePrefix(boolean UsePrefix) {
        usePrefix = UsePrefix;
    }

    /**
     * Returns is public flag
     * 
     * @return is public flag
     */

    public boolean getIsPublic() {
        return isPublic;
    }

    /**
     * Sets is public flag
     * 
     * @param IsPublic flag to set
     */
    
    public void setIsPublic(boolean IsPublic) {
        isPublic = IsPublic;
    }
    /**
     * Returns Message hash
     * 
     * @return Message hash
     */

    public byte [] getHash() {
        return hash;
    }
    
    /**
     * Returns Message hash length (to be) encoded
     * 
     * @return Message hash length
     */

    public int getHashLen() {
        return hashLen;
    }
    
    /**
     * Sets message hash.
     * 
     * @param Hash to set
     */
    
    public void setHash(byte [] Hash) {
        hash=Arrays.copyOf(Hash, Hash.length);
    }
    
    /**
     * Sets message hash length.
     * 
     * @param HashLen to set
     */
    
    public void setHashLen(int HashLen) {
        hashLen=HashLen;
    }
    
    /**
     * Returns array of output ranges
     * 
     * @return array of output ranges
     */
    
    public CoinSparkIORange [] getOutputRanges()
    {
        return Arrays.copyOf(outputRanges,countOutputRanges);
    }

    /**
     * Sets array of output ranges
     * 
     * @param OutputRanges to set
     */
    
    public void setOutputRanges(CoinSparkIORange [] OutputRanges)
    {
        countOutputRanges=OutputRanges.length;
        outputRanges=Arrays.copyOf(OutputRanges, countOutputRanges);
    }
    
    /**
     * Adds OutputRange to the list
     * 
     * @param OutputRange to add
     */
    
    public void addOutputs(CoinSparkIORange OutputRange)
    {
        if (COINSPARK_MESSAGE_MAX_IO_RANGES > countOutputRanges)
        {
            outputRanges[countOutputRanges] = OutputRange;
            countOutputRanges++;
        }        
    }
    
    /**
     * CoinSparkMessage class for managing CoinSpark messages
     */

    public CoinSparkMessage()
    {       
        clear();
    }

    /**
     * Set all fields in address to their default/zero values, which are not necessarily valid.
     */
    
    public final void clear()
    {        
        useHttps = false;
        serverHost = ""; 
        usePrefix = false; 
        serverPath = "";  
        isPublic = false;
        outputRanges = new CoinSparkIORange[COINSPARK_MESSAGE_MAX_IO_RANGES]; 
        countOutputRanges = 0;
        hash = new byte[COINSPARK_MESSAGE_HASH_MAX_LEN]; 
        hashLen = 0; 
    }
    
    @Override
    public String toString()
    {
        CoinSparkBuffer assetWebPageBuffer=new CoinSparkBuffer();
        String encodedWebPage="";
                
        CoinSparkDomainPath assetWebPage=new CoinSparkDomainPath(serverHost, serverPath, useHttps, usePrefix);
        if(assetWebPage.encode(assetWebPageBuffer))
        {
            encodedWebPage=assetWebPageBuffer.toHex();
        }
        
        String urlString=calcServerUrl();
        
        StringBuilder sb = new StringBuilder();
        sb.append("COINSPARK MESSAGE\n");
        sb.append(String.format("    Server URL: %s (length %d+%d encoded %s length %d)\n", 
                urlString,
                serverHost.length(),
                serverPath.length(),
                encodedWebPage,
                assetWebPage.encodedLen()));
        sb.append(String.format("Public message: %s\n", isPublic ? "yes" : "no"));

        for (int index=0; index < countOutputRanges; index++)
        {
            if(outputRanges[index].count > 0)
            {
                if(outputRanges[index].count > 1)
                {
                    sb.append(String.format("       Outputs: %d - %d (count %d)", 
                            outputRanges[index].first,
                            outputRanges[index].first+outputRanges[index].count-1,
                            outputRanges[index].count));                    
                }
                else
                {
                    sb.append(String.format("        Output: %d",outputRanges[index].first));
                }
                sb.append(String.format(" (small endian hex: first %s count %s)\n",
                    unsignedToSmallEndianHex(outputRanges[index].first, 2),
                    unsignedToSmallEndianHex(outputRanges[index].count, 2))); 
            }
            else
            {
                sb.append("       Outputs: none");                
            }

        }
        
        sb.append(String.format("  Message hash: "));
        sb.append(byteToHex(Arrays.copyOf(hash, hashLen)));                
        sb.append(String.format(" (length %d)\n", hashLen));

        sb.append("END COINSPARK MESSAGE\n\n");
        return sb.toString();
    }
    
    /**
     * Returns true if all values in the message are in their permitted ranges, false otherwise.
     * 
     * @return true if genesis structure is valid
     */
    
    public boolean isValid()
    {			
        if (serverHost.length() > COINSPARK_MESSAGE_SERVER_HOST_MAX_LEN)
            return false;
	
        if (serverPath.length()>COINSPARK_MESSAGE_SERVER_PATH_MAX_LEN)
            return false;
				
        if (hash.length < hashLen)                                              // check we have at least as much data as specified by $this->hashLen
            return false; 
	
        if ( (hashLen<COINSPARK_MESSAGE_HASH_MIN_LEN) || (hashLen>COINSPARK_MESSAGE_HASH_MAX_LEN) )
            return false;

        if ( (!isPublic) && (countOutputRanges == 0) )                          // public or aimed at some outputs at least
            return false;
	
        if (countOutputRanges > COINSPARK_MESSAGE_MAX_IO_RANGES)
            return false;
	
        for (int index=0; index < countOutputRanges; index++)
        {
            if(!outputRanges[index].isValid())
            {
                return false;
            }
        }

        return true;
    }
		
    /**
     * Returns true if the two CoinSparkMessage structures are the same. If strict is true then
     * the OutputRanges must be identical.
     * If strict is false then only normalized OutputRanges must be identical.
     *
     * @param message2 CoinSparkMessage to compare with
     * @param strict Strict comparison flag
     * @return true if two CoinSparkMessage match, false otherwise
     */
    
    
    public boolean match(CoinSparkMessage message2, boolean strict)
    {
        int hashCompareLen=Math.min(Math.min(hashLen, message2.getHashLen()), COINSPARK_MESSAGE_HASH_MAX_LEN);
	
        CoinSparkIORange [] thisRanges;
        CoinSparkIORange [] otherRanges;
        
        
        thisRanges = getOutputRanges();
        otherRanges = message2.getOutputRanges();
        if(!strict)
        {
            thisRanges=CoinSparkIORange.normalizeIORanges(thisRanges);
            otherRanges=CoinSparkIORange.normalizeIORanges(otherRanges);
        }
        
        if (thisRanges.length != otherRanges.length)
            return false;
				
        for (int index=0; index < thisRanges.length; index++)
        {
            if(!thisRanges[index].match(otherRanges[index]))
                return false;
        }
        
        byte [] otherHash=message2.getHash();
        for(int index=0;index <hashCompareLen;index++)
        {
            if(hash[index] != otherHash[index])
                return false;
        }
			
        return (useHttps==message2.getUseHttps()) &&
               (serverHost.equalsIgnoreCase(message2.getServerHost())) &&
               (usePrefix==message2.getUsePrefix()) &&
               (serverPath.equalsIgnoreCase(message2.getServerPath())) &&
               (isPublic==message2.getIsPublic());
    }
    
    /**
     * Encodes the message into metadata (maximal size is metadataMaxLen);
     * 
     * @param countOutputs number of outputs in transaction
     * @param metadataMaxLen maximal size of encoded data
     * @return String | null Encoded message as hexadecimal, null if we failed.
     */
    
    public String encodeToHex(int countOutputs,int metadataMaxLen)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer();
        if(!encode(buffer,countOutputs,metadataMaxLen))
        {
            return null;
        }
        
        return buffer.toHex();
    }
    
    /**
     * Encodes the message into metadata (maximal size is metadataMaxLen);
     * 
     * @param countOutputs number of outputs in transaction
     * @param metadataMaxLen maximal size of encoded data
     * @return byte [] | null Encoded message as hexadecimal, null if we failed.
     */
    
    public byte [] encode(int countOutputs,int metadataMaxLen)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer();
        
        if(!encode(buffer,countOutputs,metadataMaxLen))
        {
            return null;
        }
        
        return buffer.toBytes();
    }
    
    /**
     * Decodes the message.
     * 
     * @param countOutputs number of outputs in transaction
     * @param metadata Metadata to decode as hexadecimal
     * @return true on success, false on failure
     */
    
    public boolean decode(String metadata,int countOutputs)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata, true);
        return decode(buffer,countOutputs);
    }
    
    /**
     * Decodes the message.
     * 
     * @param countOutputs number of outputs in transaction
     * @param metadata Metadata to decode as raw data
     * @return true on success, false on failure
     */
    
    public boolean decode(byte [] metadata,int countOutputs)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata);
        return decode(buffer,countOutputs);
    }
    
    /**
     * Content part subclass.
     * 
     * All String parameters must be passed using UTF-8 encoding.
     */    
    
    public class ContentPart
    {
        public String mimeType;
        public String fileName;
        public byte [] content=new byte[0];                
    }
    
    /**
     * Calculates the hash for the specific set of ContentParts
     *
     * 
     * @return asset hash or null on failure
    */
    
    /**
     * Calculates the hash for the specific set of ContentParts
     * 
     * @param salt  salt parameter
     * @param messageParts content parts to hash
     * @return message hash or null on failure
     */
    
    public static byte [] calcMessageHash(byte [] salt, ContentPart [] messageParts)
    {
        if(salt == null)
        {
            salt = new byte [0];
        }
        
        int bufferSize=16+salt.length+messageParts.length*3;
        
        for(ContentPart part : messageParts)
        {
            bufferSize += (part.mimeType != null) ? part.mimeType.getBytes().length : 0;
            bufferSize += (part.fileName != null) ? part.fileName.getBytes().length : 0;
            bufferSize+=part.content.length;
        }
                
        byte[] buffer = new byte[bufferSize];
        int offset = 0;
        
        if(salt.length>0)
        {
            System.arraycopy(salt, 0, buffer, offset, salt.length);
        }
        offset += salt.length+1;buffer[offset-1]=0x00;
        for(ContentPart part : messageParts)
        {
            if((part.mimeType != null) && (part.mimeType.length() > 0))
            {
                System.arraycopy(part.mimeType.getBytes(), 0, buffer, offset, part.mimeType.length());
                offset += part.mimeType.length();
            }
            offset += 1;buffer[offset-1]=0x00;
            if((part.fileName != null) && (part.fileName.length() > 0))
            {
                System.arraycopy(part.fileName.getBytes(), 0, buffer, offset, part.fileName.length());
                offset += part.fileName.length();
            }
            offset += 1;buffer[offset-1]=0x00;
            if(part.content.length>0)
            {
                System.arraycopy(part.content, 0, buffer, offset, part.content.length);
            }
            offset += part.content.length+1;buffer[offset-1]=0x00;
        }        
        
        return coinSparkCalcSHA256Hash(buffer, offset);
    }
    
    /**
     * Returns true if message has specified output in its ranges
     * 
     * @param outputIndex output index to check
     * @return true if message has specified output in its ranges, false otherwise
     */
    
    public boolean hasOutput(int outputIndex)
    {
        for (int index=0; index < countOutputRanges; index++)
        {
            if ( (outputIndex>=outputRanges[index].first) && (outputIndex<(outputRanges[index].first+outputRanges[index].count)) )
                    return true;
        }
        return false;
    }
		
    /**
     * Calculates the appropriate message hash length so that when encoded as metadata the genesis will
     * fit in metadataMaxLen bytes. For now, set metadataMaxLen to 40 (see Bitcoin's MAX_OP_RETURN_RELAY parameter).
     *
     * @param metadataMaxLen metadata maximal length
     * @return asset hash length of message
     */
    
    /**
     * Calculates the appropriate message hash length so that when encoded as metadata the genesis will
     * fit in metadataMaxLen bytes.For now, set metadataMaxLen to 40 (see Bitcoin's MAX_OP_RETURN_RELAY parameter).
     * 
     * @param countOutputs number of outputs in transaction
     * @param metadataMaxLen metadata maximal length
     * @return hash length of the message
     */
    
    public int calcHashLen(int countOutputs, int metadataMaxLen)
    {
        int len = metadataMaxLen-COINSPARK_METADATA_IDENTIFIER.length()-1;
	
        CoinSparkDomainPath assetWebPage=new CoinSparkDomainPath(serverHost, serverPath, useHttps, usePrefix);
        len-=assetWebPage.encodedLen();
	
	if (isPublic)
            len--;
		
        for (int index=0; index < countOutputRanges; index++)
        {
            int [] result = getOutputRangePacking(outputRanges[index], countOutputs);
            
            if(result != null)
            {
                if(result[0]>0)
                {
                    len -= (1 + result[1] +result[2]);
                }
            }
        }        
        
        if (len > COINSPARK_MESSAGE_HASH_MAX_LEN)
            len = COINSPARK_MESSAGE_HASH_MAX_LEN;

        return len;
    }
		
    
    
// Private variables/constants/functions   
        
    private boolean useHttps;
    private String serverHost; 
    private boolean usePrefix; 
    private String serverPath;  
    private boolean isPublic;
    CoinSparkIORange [] outputRanges; 
    private int countOutputRanges;
    private byte[] hash; 
    private int hashLen; 
    

    private static final int COINSPARK_MESSAGE_SERVER_HOST_MAX_LEN=32;
    private static final int COINSPARK_MESSAGE_SERVER_PATH_MAX_LEN=24;
    private static final int COINSPARK_MESSAGE_HASH_MIN_LEN=12;
    private static final int COINSPARK_MESSAGE_HASH_MAX_LEN=32;
    private static final int COINSPARK_MESSAGE_MAX_IO_RANGES=16;


    private static final int COINSPARK_OUTPUTS_MORE_FLAG=0x80;
    private static final int COINSPARK_OUTPUTS_RESERVED_MASK=0x60;
    private static final int COINSPARK_OUTPUTS_TYPE_MASK=0x18;
    private static final int COINSPARK_OUTPUTS_TYPE_SINGLE=0x00; // one output index (0...7)
    private static final int COINSPARK_OUTPUTS_TYPE_FIRST=0x08; // first (0...7) outputs
    private static final int COINSPARK_OUTPUTS_TYPE_UNUSED=0x10; // for future use
    private static final int COINSPARK_OUTPUTS_TYPE_EXTEND=0x18; // "extend", including public/all
    private static final int COINSPARK_OUTPUTS_VALUE_MASK=0x07;
    private static final int COINSPARK_OUTPUTS_VALUE_MAX=7;

    private String calcServerUrl()
    {
        String s="";
        
        s += useHttps ? "https" : "http";
        s += "://" + serverHost + "/";
        s += usePrefix ? "coinspark/" : "";
        s += serverPath;
        s += (serverPath.length()>0) ? "/" : "";
                        
        return s.toLowerCase();
    }

    private int [] getOutputRangePacking(CoinSparkIORange outputRange, int countOutputs)
    {
        int [] result=new int[3];
        int packing;
        Byte packingExtend;
        
        boolean [] packingOptions=CoinSparkPacking.getPackingOptions(null, outputRange, countOutputs, true);

        result[1]=0;
        result[2]=0;

        if (packingOptions[CoinSparkPacking.PackingType._1_0_BYTE.getValue()] && (outputRange.first<=COINSPARK_OUTPUTS_VALUE_MAX)) // inline single output
            packing=COINSPARK_OUTPUTS_TYPE_SINGLE | (outputRange.first & COINSPARK_OUTPUTS_VALUE_MASK);

        else if (packingOptions[CoinSparkPacking.PackingType._0_1_BYTE.getValue()] && (outputRange.count<=COINSPARK_OUTPUTS_VALUE_MAX)) // inline first few outputs
            packing=COINSPARK_OUTPUTS_TYPE_FIRST | (outputRange.count & COINSPARK_OUTPUTS_VALUE_MASK);

        else 
        {                                                                       // we'll be taking additional bytes
            packingExtend=CoinSparkPacking.encodePackingExtend(packingOptions);
            if (packingExtend == null)
                return null;

            result=CoinSparkPacking.packingExtendAddByteCounts(packingExtend, result[1], result[2]);

            packing=COINSPARK_OUTPUTS_TYPE_EXTEND | (packingExtend & COINSPARK_OUTPUTS_VALUE_MASK);
        }

        result[0]=packing;
        return result;
    }		
    
    private boolean encode(CoinSparkBuffer buffer,int countOutputs,int metadataMaxLen)
    {
        int packing,packingExtend;
        
        try
        {
            if (!isValid())
                throw new CoinSparkExceptions.CannotEncode("invalid message");
        
            //  4-character identifier

            buffer.writeString(COINSPARK_METADATA_IDENTIFIER);
            buffer.writeByte(COINSPARK_MESSAGE_PREFIX);
		
            //  Server host and path

            CoinSparkDomainPath assetWebPage=new CoinSparkDomainPath(serverHost, serverPath, useHttps, usePrefix);
            if (!assetWebPage.encode(buffer))
                throw new CoinSparkExceptions.CannotEncode("cannot write domain name/path");
	
            //  Output ranges
	
            if (isPublic) 
            {                                                                   // add public indicator first
                packing=((countOutputRanges>0) ? COINSPARK_OUTPUTS_MORE_FLAG : 0) |
                                COINSPARK_OUTPUTS_TYPE_EXTEND | CoinSparkPacking.COINSPARK_PACKING_EXTEND_PUBLIC;
                buffer.writeInt(packing, 1);
            }
			
            for (int index=0; index < countOutputRanges; index++)
            {
                int firstBytes,countBytes;

                int [] result=getOutputRangePacking(outputRanges[index], countOutputs);
                if(result == null)
                    throw new CoinSparkExceptions.CannotEncode("invalid range");
                
                packing=result[0];
                firstBytes=result[1];
                countBytes=result[2];                
                
                //  The packing byte

                if ((index+1)<countOutputRanges)
                    packing |= COINSPARK_OUTPUTS_MORE_FLAG;

                buffer.writeInt(packing, 1);
		
                buffer.writeInt(outputRanges[index].first, firstBytes);
                buffer.writeInt(outputRanges[index].count, countBytes);                
            }
            
            //  Message hash
            
            buffer.writeBytes(hash, hashLen);
            
            if(buffer.length()>metadataMaxLen)
                throw new CoinSparkExceptions.CannotEncode("total length above limit");		
        
        }
        catch (Exception ex)
        {
            System.out.print(ex.getMessage());
            return false;
        }
        
        return true;
    }    
    
    private boolean decode(CoinSparkBuffer buffer,int countOutputs)
    {
        if(!buffer.locateRange(COINSPARK_MESSAGE_PREFIX))
            return false;

        try
        {            
            //  Server host and path
            CoinSparkDomainPath assetWebPage=new CoinSparkDomainPath(serverHost, serverPath, useHttps, usePrefix);
            if (!assetWebPage.decode(buffer))
                throw new CoinSparkExceptions.CannotDecode("cannot decode server host");
            
            serverHost=assetWebPage.domainName;
            serverPath=assetWebPage.path;
            useHttps=assetWebPage.useHttps;
            usePrefix=assetWebPage.usePrefix;
            
            //  Output ranges
	
            isPublic=false;
            outputRanges=new CoinSparkIORange[COINSPARK_MESSAGE_MAX_IO_RANGES];
            countOutputRanges=0;
	            
            int packing = COINSPARK_OUTPUTS_MORE_FLAG;            
            
            while((packing & COINSPARK_OUTPUTS_MORE_FLAG) > 0)
            {                
                if(buffer.canRead(1))
                {
                    packing=buffer.readInt(1);                                  //  Read the next packing byte and check reserved bits are zero
                }
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read packing");

                if((packing & COINSPARK_OUTPUTS_RESERVED_MASK) > 0)
                    throw new CoinSparkExceptions.CannotDecode("reserved bits used in packing");
                		
		int packingType=packing & COINSPARK_OUTPUTS_TYPE_MASK;
                int packingValue=packing & COINSPARK_OUTPUTS_VALUE_MASK;
		
		if ((packingType==COINSPARK_OUTPUTS_TYPE_EXTEND) && (packingValue==CoinSparkPacking.COINSPARK_PACKING_EXTEND_PUBLIC))
                {
                    isPublic=true;                                              // special case for public messages
                }
                else 
                {		
                                                                                //  Create a new output range			
                    if (countOutputRanges>=COINSPARK_MESSAGE_MAX_IO_RANGES)     // too many output ranges
                        throw new CoinSparkExceptions.CannotDecode("too many output ranges");
					
                    int firstBytes=0;
                    int countBytes=0;			
                    CoinSparkIORange outputRange;
                                                                                //  Decode packing byte			
                    if (packingType==COINSPARK_OUTPUTS_TYPE_SINGLE)             // inline single input
                    {                                                           
                        outputRange=new CoinSparkIORange();
                        outputRange.first=packingValue;
                        outputRange.count=1;
                    } 
                    else if (packingType==COINSPARK_OUTPUTS_TYPE_FIRST)         // inline first few outputs
                    { 
                        outputRange=new CoinSparkIORange();
                        outputRange.first=0;
                        outputRange.count=packingValue;
                    } 
                    else if (packingType==COINSPARK_OUTPUTS_TYPE_EXTEND)        // we'll be taking additional bytes
                    { 
                        CoinSparkPacking.PackingType extendPackingType;
			extendPackingType=CoinSparkPacking.decodePackingExtend((byte)packingValue, true);
                        if (extendPackingType == CoinSparkPacking.PackingType._NONE)
                            throw new CoinSparkExceptions.CannotDecode("Wrong packing type");
						
                        outputRange=CoinSparkPacking.packingTypeToValues(extendPackingType, null, countOutputs);

                        int result [] =CoinSparkPacking.packingExtendAddByteCounts(packingValue, firstBytes, countBytes);
                        firstBytes = result[1];				
                        countBytes = result[2];				
                    } 
                    else
                        throw new CoinSparkExceptions.CannotDecode("unused packing type");
                        
			
                    //  The index of the first output and number of outputs, if necessary
					
                    if (firstBytes>0)
                    {
                        if(buffer.canRead(firstBytes))
                            outputRange.first=buffer.readInt(firstBytes);
                        else
                            throw new CoinSparkExceptions.CannotDecode("Cannot read first");
                    }

                    if (countBytes>0)
                    {
                        if(buffer.canRead(countBytes))
                            outputRange.count=buffer.readInt(countBytes);
                        else
                            throw new CoinSparkExceptions.CannotDecode("Cannot read count");
                    }
                    
                    outputRanges[countOutputRanges]=outputRange;			//	Add on the new output range
				
                    countOutputRanges++;
                }
                
            }
            
            //  Message hash

            hashLen = buffer.availableForRead();//TBD loss
            hashLen = Math.min(hashLen, COINSPARK_MESSAGE_HASH_MAX_LEN);        // apply maximum

            if (hashLen < COINSPARK_MESSAGE_HASH_MIN_LEN)                       // not enough hash data                
                throw new CoinSparkExceptions.CannotDecode("has data out of range");

            hash=buffer.readBytes(hashLen);            
        }
        catch (Exception ex)
        {
            System.out.print(ex.getMessage());
            return false;
        }
        
        return isValid();
        
			
	
    }

    
}
