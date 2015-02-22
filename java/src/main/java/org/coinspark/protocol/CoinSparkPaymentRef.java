/*
 * CoinSpark 2.1 - Java library
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

import java.util.Random;

/**
 * Class for managing CoinSpark payment references.
 */

public class CoinSparkPaymentRef  extends CoinSparkBase
{
    public static final long COINSPARK_PAYMENT_REF_MAX  = 0xFFFFFFFFFFFFFL; // 2^52-1

    /**
     * Payment reference long (64-bit value)
     */
    
    public long ref = 0;

//	Public functions
    
    /**
     * Class for managing CoinSpark payment references.
     */
    
    public CoinSparkPaymentRef()
    {
        ref = 0;
    }
    
    /**
     * Class for managing CoinSpark payment references.
     * @param Ref Long (64-bit) value to set
     */
    
    public CoinSparkPaymentRef(long Ref)
    {
        ref = Ref;
    }

    /**
     * Returns CoinSpark Payment reference value.
     * 
     * @return CoinSpark Payment reference value
     */
    
    public long getRef()
    {
        return ref;
    }
    
    
    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder();
        long temp = ref;

        sb.append("COINSPARK PAYMENT REFERENCE\n")
        .append(String.format("%d (small endian hex %s)\n", this.ref, unsignedToSmallEndianHex(temp, 8)))
        .append("END COINSPARK PAYMENT REFERENCE\n\n");

        return sb.toString();
    }
    
    /**
     * Returns true if paymentRef is in the permitted range, false otherwise.
     * 
     * @return true if paymentRef is in the permitted range, false otherwise
     */
    
    public boolean isValid()
    {
        return ((ref >=0) && (ref<=COINSPARK_PAYMENT_REF_MAX));
    }

    /**
     * Compares two payment references
     * @param Ref2 Payment reference to compare with 
     * @return true is two payment references match, false otherwise
     */
    
    public boolean match(CoinSparkPaymentRef Ref2)
    {
        return (ref == Ref2.ref);
    }

    /**
     * Returns a random payment reference that can be used for a CoinSpark address and embedded in a transaction.
     * 
     * @return a random payment reference that can be used for a CoinSpark address and embedded in a transaction.
     */
    
    public CoinSparkPaymentRef randomize()
    {
        long paymentRef = 0;
        long bitsRemaining = COINSPARK_PAYMENT_REF_MAX;

        Random rnd=new Random();
        
        while(bitsRemaining > 0)
        {
            paymentRef <<= 13;
            paymentRef |= rnd.nextInt(0x2000);
            bitsRemaining >>= 13;            
        }

        ref=paymentRef % (1+COINSPARK_PAYMENT_REF_MAX);
        
        return this;
    }
        
    /**
     * Encodes the paymentRef into metadata (maximal size is CoinSparkBase.OP_RETURN_MAXIMUM_LENGTH);
     * 
     * @return String | null Encoded payment reference as hexadecimal, null if we failed.
     */
    /*
    public String encodeToHex()
    {
        return encodeToHex(OP_RETURN_MAXIMUM_LENGTH);
    }
    */
    /**
     * Encodes the paymentRef into metadata (maximal size is metadataMaxLen);
     * 
     * @param metadataMaxLen maximal size of encoded data
     * @return String | null Encoded payment reference as hexadecimal, null if we failed.
     */
    
    public String encodeToHex(int metadataMaxLen)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer();
        if(!encode(buffer,metadataMaxLen))
        {
            return null;
        }
        
        return buffer.toHex();
    }
    
    /**
     * Encodes the paymentRef into metadata (maximal size is CoinSparkBase.OP_RETURN_MAXIMUM_LENGTH);
     * 
     * @return byte [] | null Encoded payment reference as raw data, null if we failed.
     */
    /*    
    public byte [] encode()
    {        
        return encode(OP_RETURN_MAXIMUM_LENGTH);
    }
    */
    /**
     * Encodes the paymentRef into metadata (maximal size is metadataMaxLen);
     * 
     * @param metadataMaxLen maximal size of encoded data
     * @return byte [] | null Encoded payment reference as hexadecimal, null if we failed.
     */
    
    public byte [] encode(int metadataMaxLen)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer();
        
        if(!encode(buffer,metadataMaxLen))
        {
            return null;
        }
        
        return buffer.toBytes();
    }
    
    /**
     * Decodes the payment reference in metadata  into paymentRef.
     * 
     * @param metadata Metadata to decode as hexadecimal
     * @return true on success, false on failure
     */
    
    public boolean decode(String metadata)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata, true);
        return decode(buffer);
    }
    
    /**
     * Decodes the payment reference in metadata  into paymentRef.
     * 
     * @param metadata Metadata to decode as raw data
     * @return true on success, false on failure
     */
    
    public boolean decode(byte [] metadata)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata);
        return decode(buffer);
    }
    
    
    
//	Private functions
        
    
    private boolean encode(CoinSparkBuffer buffer,int metadataMaxLen)
    {
        try
        {
            if ( !isValid() )
                throw new CoinSparkExceptions.CannotEncode("invalid payment reference");

            buffer.writeString(COINSPARK_METADATA_IDENTIFIER);                  // CoinSpark metadata identifier
            buffer.writeByte(COINSPARK_PAYMENTREF_PREFIX);                      // CoinSpark metadate prefix
            
            int bytes=0;
            long left = ref;
            
            while (left > 0)   // do I need all these? i can use toString
            {
                left >>= 8;
                bytes++;
            }

            buffer.writeLong(ref, bytes);                                     // payment reference
            
            if(buffer.length()>metadataMaxLen)                                  // check the total length is within the specified limit
                throw new CoinSparkExceptions.CannotEncode("total length above limit");
        }
        catch (Exception ex)
        {
            System.out.print(ex.getMessage());
            return false;
        }
        
        return true;
    }
    
    
    private boolean decode(CoinSparkBuffer buffer)
    {
        if(!buffer.locateRange(COINSPARK_PAYMENTREF_PREFIX))
            return false;
        
        try
        {            
            ref=buffer.readLong(buffer.availableForRead());                   // Payment reference
            
            if (!isValid())
                throw new CoinSparkExceptions.CannotDecode("Payment  is invalid");
        }
        
        catch (Exception ex)
        {
            System.out.print(ex.getMessage());
            return false;
        }
        
        return true;
    }    
}
