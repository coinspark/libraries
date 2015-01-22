/*
 * CoinSpark 2.0 - Java library
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

import java.io.UnsupportedEncodingException;
import java.util.Arrays;


/**
 * CoinSpark class for managing Asset web pages structures
 */

public class CoinSparkDomainPath extends CoinSparkBase{

    /**
     * Doiman name (without scheme).
     */
    
    protected String domainName;
    
    /**
     * Path used in asset web page URL construction, may be null. 
     * If null Spent txID and Vout are used.
     */
    
    protected String path;
    
    /**
     * Use Https flag.
     */
    
    protected boolean useHttps;
    
    /**
     * Use "coinspark/" prefix in path flag.
     */
    
    protected boolean usePrefix;

    /**
     * CoinSpark class for managing Asset web pages structures
     */
    
    protected CoinSparkDomainPath()
    {
        
    }
    
    /**
     * CoinSpark class for managing Asset web pages structures
     * 
     * @param DomainName domain name to set
     * @param Path path to set (may be null)
     * @param UseHttps use https
     * @param UsePrefix use "coinspark/" prefix
     */
    
    public CoinSparkDomainPath(String DomainName,String Path, boolean UseHttps, boolean UsePrefix)
    {
        domainName=DomainName;
        path=Path;
        useHttps=UseHttps;
        usePrefix=UsePrefix;
    }

    /**
     * Calculates the URL.
     * 
     * @return String FUll Domain/path URL 
     */
    
    public String getFullURL()
    {
        return String.format("%s://%s/%s%s/", useHttps ? "https" : "http",domainName,usePrefix ? "coinspark/" : "",path).toLowerCase();
    }
    
    /**
     * Returns true if all values in the asset web page are in their permitted ranges, false otherwise.
     * 
     * @return true if asset web page structure is valid
     */
    
    protected boolean isValid()
    {
        if ((domainName.length() > CoinSparkGenesis.COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN) || (path.length() > CoinSparkGenesis.COINSPARK_GENESIS_PAGE_PATH_MAX_LEN))
            return false;
                
        return true;
    }
    
    /**
     * Compares two CoinSpark asset web page structures references
     *  
     * @param page2 Asset web page structure to compare with
     * @return Returns true if the two Asset web page structures are identical.
     */
    
    protected boolean match(CoinSparkDomainPath page2)
    {
        return 
                domainName.toLowerCase().equals(page2.domainName.toLowerCase()) && 
                path.toLowerCase().equals(page2.path.toLowerCase()) &&
                (useHttps == page2.useHttps) && 
                (usePrefix == page2.usePrefix);
    }
    
    /**
     * Calculates the URL for the asset web page of genesis.
     * 
     * @param FirstSpentTxID if path=null or path.length=0 pass the previous txid whose output was spent by the first input of the genesis
     * @param FirstSpentVout if path=null or path.length=0 pass the output index of firstSpentTxID spent by the first input of the genesis
     * @return String | null URL of the Asset web page, null on failure
     */
    
    protected String getAssetURL(String FirstSpentTxID,long FirstSpentVout)
    {
        if(domainName == null)
        {
            return null;
        }
        
        String suffix=path;

        
        if((suffix == null) || (suffix.length() == 0))
        {
            if(FirstSpentTxID.length() != 64)
            {
                return null;
            }
            int start= (int)(FirstSpentVout % 64);
            int charsToCopy=Math.min(16, 64-start);
            suffix=FirstSpentTxID.substring(start, start+charsToCopy);
            if(charsToCopy<16)
            {
                suffix+=FirstSpentTxID.substring(0,16-charsToCopy);
            }
        }
        
        return String.format("%s://%s/%s%s/", useHttps ? "https" : "http",domainName,usePrefix ? "coinspark/" : "",suffix).toLowerCase();
    }
            
    /**
     * Calculates the URL for the home page, based on domain and useHttps flag
     * 
     * @return String | null URL of the domain home page, null on failure
     */
    
    protected String getDomainURL()
    {        
        if(domainName == null)
        {
            return null;
        }
        
        return String.format("%s://%s", useHttps ? "https" : "http",domainName);
    }
        
// Private variables/constants/functions       
    
    
    private static final int COINSPARK_DOMAIN_PACKING_PREFIX_MASK     = 0xC0;
    private static final int COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT    = 6;
    private static final byte COINSPARK_DOMAIN_PACKING_SUFFIX_MASK     = 0x3F;
    private static final int COINSPARK_DOMAIN_PACKING_SUFFIX_MAX      = 61;
    private static final int COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4_NO_PATH  = 62; // messages only
    private static final int COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4     = 63;
    private static final int COINSPARK_DOMAIN_PACKING_IPv4_HTTPS      = 0x40;
    private static final int COINSPARK_DOMAIN_PACKING_IPv4_NO_PATH_PREFIX   = 0x80;
    private static final int COINSPARK_DOMAIN_PATH_ENCODE_BASE        = 40;
    private static final int COINSPARK_DOMAIN_PATH_FALSE_MARKER       = 38;
    private static final int COINSPARK_DOMAIN_PATH_TRUE_MARKER        = 39;

    private static final String domainNamePrefixes[]={
            "",
            "www."
    };
    private static final String domainNameSuffixes[]={      // 60, // leave space for 3 more in future
            "",

            // most common suffixes based on Alexa's top 1M sites as of 10 June 2014, with some manual adjustments

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
    };

    private static final String domainNameChars = "0123456789abcdefghijklmnopqrstuvwxyz-.<>"; 
   
    private String domainNameShort;
    private int domainNamePacking;    
    
        
    private boolean shrink()
    {
        int bestPrefixLen=0;
        int bestPrefix=0;
        int bestSuffixLen=0;
        int bestSuffix=0;
        
        String source=domainName.toLowerCase();
        
        for (int prefixIndex=1; prefixIndex < domainNamePrefixes.length; prefixIndex++)
        {
            int prefixLen = domainNamePrefixes[prefixIndex].length();

            if(source.length()>prefixLen)
            {
                if (prefixLen>bestPrefixLen && domainNamePrefixes[prefixIndex].equals(source.substring(0, prefixLen)))
                {
                    bestPrefix=prefixIndex;
                    bestPrefixLen=prefixLen;
                }
            }
        }
        
        domainNameShort=source.substring(bestPrefixLen);
        
        for (int suffixIndex=1; suffixIndex < domainNameSuffixes.length; suffixIndex++)
        {
            int suffixLen = domainNameSuffixes[suffixIndex].length();

            if(domainNameShort.length()>suffixLen)
            {
                if (suffixLen>bestSuffixLen && domainNameSuffixes[suffixIndex].equals(domainNameShort.substring(domainNameShort.length()-suffixLen)))
                {
                    bestSuffix=suffixIndex;
                    bestSuffixLen=suffixLen;
                }
            }
        }
        
        domainNameShort=domainNameShort.substring(0, domainNameShort.length()-bestSuffixLen);
            
        domainNamePacking=((bestPrefix << COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT)&COINSPARK_DOMAIN_PACKING_PREFIX_MASK)|
                 (bestSuffix & COINSPARK_DOMAIN_PACKING_SUFFIX_MASK);
        
        return true;
    }

    private boolean expand()
    {
        int prefixIndex = (domainNamePacking & COINSPARK_DOMAIN_PACKING_PREFIX_MASK)>>COINSPARK_DOMAIN_PACKING_PREFIX_SHIFT;
        int suffixIndex = domainNamePacking & COINSPARK_DOMAIN_PACKING_SUFFIX_MASK;
        
        domainName = domainNamePrefixes[prefixIndex] + domainNameShort +  domainNameSuffixes[suffixIndex];
        
        return true;        
    }
    
    private int[] octetsIPV4()
    {
        int [] octets=new int[4];
        int octetNum, octetValue;
        char stringChar = 0;
        int i = 0;

        for (octetNum=0; octetNum<4; octetNum++)
        {
            octetValue=0;

            while (i < domainName.length())
            {
                stringChar = domainName.charAt(i++);

                if ((stringChar>='0') && (stringChar<='9')) {
                    octetValue=octetValue*10+(stringChar-'0');
                    if (octetValue>255)
                        return null;

                }
                else if ((stringChar=='.') || (i >= domainName.length()))
                {
                    break;
                }
                else
                    return null;
            }

            octets[octetNum] = octetValue;
            if (octetNum < 3 && stringChar != '.')
                return null;

            if (octetNum == 3 && i != domainName.length())
                return null;
        }

        return octets;        
    }
    
    private boolean packString(CoinSparkBuffer buffer,String Source)
    {
        int strPos, strTriplet, strChar, strLen;
        
        strLen=Source.length();
        try
        {        
            strTriplet=0;
            for (strPos = 0; strPos<Source.length(); strPos++) {

                int foundChar = domainNameChars.indexOf(Character.toLowerCase(Source.charAt(strPos)));
                if (foundChar == -1)
                    throw new CoinSparkExceptions.CannotEncode("Invalid character in packing source");
                strChar = foundChar;

                switch (strPos%3)
                {
                    case 0:
                        strTriplet=strChar;
                        break;

                    case 1:
                        strTriplet+=strChar*COINSPARK_DOMAIN_PATH_ENCODE_BASE;
                        break;

                    case 2:
                        strTriplet+=strChar*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE;
                        break;
                }

                if ( ((strPos%3)==2) || (strPos==Source.length()-1) ) // write out 2 bytes if we've collected 3 chars, or if we're finishing
                { 
                    buffer.writeBytes(unsignedToSmallEndianBytes(strTriplet, 2));
                }
            }
        }
        catch (Exception cne)
        {
            System.out.println(cne.getMessage());
            return false;
        }
        return true;
    }
    
    private byte [] unpackString(CoinSparkBuffer buffer,int parts)
    {
        int strPos, strTriplet, strChar;
        byte [] result=new byte[256];
        
        strTriplet=0;
        strChar=0;
        strPos=0;
        try
        {
            while(parts>0)    
            {
                if(strPos>=result.length)
                {
                    throw new CoinSparkExceptions.CannotDecode("Domain/Path too long");                    
                }
                if ((strPos%3)==0)
                {
                    if(buffer.canRead(2))
                    {
                        strTriplet=buffer.readInt(2);
                        if (strTriplet >=
                                (COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE))
                            throw new CoinSparkExceptions.CannotDecode("Invalid value in Domain/Path");
                    }
                    else
                        throw new CoinSparkExceptions.CannotDecode("Ran out of characters");
                }

                switch (strPos%3)
                {
                    case 0:
                        strChar=strTriplet%COINSPARK_DOMAIN_PATH_ENCODE_BASE;
                        break;

                    case 1:
                        strChar=(strTriplet/COINSPARK_DOMAIN_PATH_ENCODE_BASE)%COINSPARK_DOMAIN_PATH_ENCODE_BASE;
                        break;

                    case 2:
                        strChar=strTriplet/(COINSPARK_DOMAIN_PATH_ENCODE_BASE*COINSPARK_DOMAIN_PATH_ENCODE_BASE);
                        break;
                }

                if ((strChar==COINSPARK_DOMAIN_PATH_FALSE_MARKER) || (strChar == COINSPARK_DOMAIN_PATH_TRUE_MARKER)) 
                {
                    parts--;
                }

                result[strPos] = (byte)domainNameChars.charAt(strChar);
                strPos++;
            }
        }
        catch (Exception cne)
        {
            System.out.println(cne.getMessage());
            return null;
        }
        
        return  Arrays.copyOfRange(result, 0, strPos);
    }
    
    
    protected int encodedLen(boolean forMessages)
    {
        int encodedLen=0;                
        int decodedLen=path.length()+1;
        
        if(octetsIPV4() != null)
        {
            encodedLen+=5;
            if(forMessages)
            {
                if(decodedLen==1)// will skip server path in this case
                {
                    decodedLen=0;
                }
            }
        }
        else
        {
            encodedLen+=1;
            shrink();
            decodedLen+=domainNameShort.length()+1;            
        }
        
        if(decodedLen>0)
        {
            encodedLen+=2*((decodedLen-1)/3+1);
        }
        
        return encodedLen;
    }
    
    protected boolean encode(CoinSparkBuffer buffer,boolean forMessages)
    {
        int [] octets;
        int parts=0;
        boolean takePathPart=true;
        String stringToPack="";
        
        try
        {
            if( (octets=octetsIPV4()) != null)
            {
                byte temp = COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4;
                if(forMessages  && (path.length() == 0))
                {
                    temp = COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4_NO_PATH;
                    temp += (usePrefix ? COINSPARK_DOMAIN_PACKING_IPv4_NO_PATH_PREFIX : (byte)0);
                    takePathPart=false;
                }
                
                temp += (useHttps ? COINSPARK_DOMAIN_PACKING_IPv4_HTTPS : (byte)0);
                
                buffer.writeByte(temp);

                buffer.writeByte((byte)octets[0]);
                buffer.writeByte((byte)octets[1]);
                buffer.writeByte((byte)octets[2]);
                buffer.writeByte((byte)octets[3]);
            }
            else
            {
                if(!shrink())
                    throw new CoinSparkExceptions.CannotEncode("Cannot shrink domain");
                        
                if (domainNameShort.length() == 0)
                    throw new CoinSparkExceptions.CannotEncode("Zero-length packing source string");

                buffer.writeByte((byte)domainNamePacking);

                stringToPack+=domainNameShort + 
                        (useHttps ? domainNameChars.charAt(COINSPARK_DOMAIN_PATH_TRUE_MARKER) : domainNameChars.charAt(COINSPARK_DOMAIN_PATH_FALSE_MARKER));
                parts++;
            }
            
            if(takePathPart)
            {
                stringToPack+=path.toLowerCase() + 
                        (usePrefix ? domainNameChars.charAt(COINSPARK_DOMAIN_PATH_TRUE_MARKER) : domainNameChars.charAt(COINSPARK_DOMAIN_PATH_FALSE_MARKER));
                parts++;
            }           
            if(parts > 0)
            {
                if(!packString(buffer, stringToPack))
                {
                    throw new CoinSparkExceptions.CannotEncode("Cannot write domain and path");                    
                }
            }
        }
        catch (Exception cne)
        {
            System.out.println(cne.getMessage());
            return false;
        }
        return true;
    }
            
    
    protected boolean decode(CoinSparkBuffer buffer,boolean forMessages)
    {
        byte packing,packingSuffix;
        int[] octets = new int[4];
        byte result[] = new byte[1];
        int parts=0;
        int pathPart;
        
        try
        {
            if (buffer.canRead(1))
            {
                packing=buffer.readByte();
            }
            else
                throw new CoinSparkExceptions.CannotDecode("Buffer is empty");

            packingSuffix=(byte)(packing & COINSPARK_DOMAIN_PACKING_SUFFIX_MASK);
            if ((packingSuffix==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4) ||
                (forMessages && (packingSuffix==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4_NO_PATH)))// check for IPv4 address
            {
                domainNamePacking=-1;
                useHttps = ((packing & COINSPARK_DOMAIN_PACKING_IPv4_HTTPS) != 0);

                if(buffer.canRead(4))
                {
                    for(int i=0;i<4;i++)
                    {
                        octets[i]=buffer.readByte();
                        if(octets[i]<0)
                            octets[i]+=256;
                    }
                }
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read octets");

                domainName = String.format("%d.%d.%d.%d", octets[0], octets[1], octets[2], octets[3]);

                if (domainName.length() >= 256) // allow for null terminator
                    throw new CoinSparkExceptions.CannotDecode("Domain name too long");
                
                if(forMessages && (packingSuffix==COINSPARK_DOMAIN_PACKING_SUFFIX_IPv4_NO_PATH))
                {
                    path="";
                    usePrefix = ((packing & COINSPARK_DOMAIN_PACKING_IPv4_NO_PATH_PREFIX) != 0);
                    parts--;
                }
            }
            else
            {
                domainNamePacking=packing;
                parts++;
            }
            
            parts++;
            pathPart=parts;
            
            byte [] unpacked=unpackString(buffer,parts);
            if(unpacked == null)
            {
                throw new CoinSparkExceptions.CannotDecode("Cannot unpack path");                    
            }
            
            byte charTrue=(byte)domainNameChars.charAt(COINSPARK_DOMAIN_PATH_TRUE_MARKER);
            byte charFalse=(byte)domainNameChars.charAt(COINSPARK_DOMAIN_PATH_FALSE_MARKER);
            
            int start=0;
            if(parts>0)
            {
                parts=1;
                for(int i=0;i<unpacked.length;i++)
                {
                    if((unpacked[i] == charTrue) || (unpacked[i] == charFalse))
                    {
                        String decodedString="";
                        boolean decodeFlag=(unpacked[i] == charTrue);
                        if(i>start)
                        {
                            decodedString=new String(Arrays.copyOfRange(unpacked, start, i), "UTF-8");
                        }
                        if(parts == pathPart)
                        {
                            path=decodedString;
                            usePrefix=decodeFlag;
                        }
                        else
                        {
                            domainNameShort=decodedString;
                            useHttps=decodeFlag;
                            expand();                        
                        }
                        start=i+1;
                        parts++;
                    }
                }            
            }
        }
        catch (CoinSparkExceptions.CannotDecode | UnsupportedEncodingException ex)
        {
           System.out.print(ex.getMessage());
           return false;
        }

        return true;
    }
}
