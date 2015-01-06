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

/**
 * CoinSparkAddress class for managing CoinSpark addresses
 */

public class CoinSparkAddress extends CoinSparkBase{

// Public functions    

    /**
     * Address supports assets if a (flags & COINSPARK_ADDRESS_FLAG_ASSETS != 0)
     */
    
    public static final int  COINSPARK_ADDRESS_FLAG_ASSETS          = 1;
    
    /**
     * Address supports payment references if a (flags & COINSPARK_ADDRESS_FLAG_PAYMENT_REFS != 0)
     */
    
    public static final int  COINSPARK_ADDRESS_FLAG_PAYMENT_REFS    = 2;

    /**
     * Address supports text messages if a (flags & COINSPARK_ADDRESS_FLAG_TEXT_MESSAGES != 0)
     */
    
    public static final int  COINSPARK_ADDRESS_FLAG_TEXT_MESSAGES    = 4;
    
    /**
     * Address supports file messages if a (flags & COINSPARK_ADDRESS_FLAG_FILE_MESSAGES != 0)
     */
    
    public static final int  COINSPARK_ADDRESS_FLAG_FILE_MESSAGES    = 8;
    
    
    /**
     * (flags & COINSPARK_ADDRESS_FLAG_MASK) is used 
     */
    
    public static final int  COINSPARK_ADDRESS_FLAG_MASK            = 0x7FFFFF; // 23 bits are currently usable
    
    /**
     * Returns bitcoin address
     * @return 
     */
    
    public String getBitcoinAddress() {
        return bitcoinAddress.toString();
    }

    /**
     * Sets bitcoin address
     * 
     * @param BitcoinAddress bitcoin address to set
     */
    
    public final void setBitcoinAddress(String BitcoinAddress) {
        bitcoinAddress = new StringBuffer(BitcoinAddress);
    }

    /**
     * Returns asset flags
     * 
     * @return Asset flags
     */
    
    public int getAddressFlags() {
        return addressFlags;
    }

    /**
     * Set address flags
     * @param AddressFlags address flags to set
     */
    
    public final void setAddressFlags(int AddressFlags) {
        addressFlags = AddressFlags;
    }

    /**
     * Returns payment reference.
     * 
     * @return Payment reference
     */
    
    public CoinSparkPaymentRef getPaymentRef() {
        return paymentRef;
    }

    /**
     * Set s payment reference
     * @param PaymentRef Payment reference to set
     */
    
    public final void setPaymentRef(CoinSparkPaymentRef PaymentRef) {
        paymentRef = PaymentRef;
    }

    
    /**
     * CoinSparkAddress class for managing CoinSpark addresses
     */
    
    public CoinSparkAddress()
    {       
        clear();
    }

    /**
     * Set all fields in address to their default/zero values, which are not necessarily valid.
     */
    
    public final void clear()
    {        
        bitcoinAddress = new StringBuffer();
        addressFlags = 0;
        paymentRef = new CoinSparkPaymentRef(0);
    }
    
    /**
     * CoinSparkAddress class for managing CoinSpark addresses
     * 
     * @param BitcoinAddress bitcoin address to set
     * @param AddressFlags  address flags to set
     * @param PaymentRef Payment reference to set
     */    
    
    public CoinSparkAddress(String BitcoinAddress, int AddressFlags, CoinSparkPaymentRef PaymentRef)
    {
        clear();
        setBitcoinAddress(BitcoinAddress);
        setAddressFlags(AddressFlags);
        setPaymentRef(PaymentRef);
    }
    
    
    @Override
    public String toString()
    {
        FlagToString[] flagsToStrings= new FlagToString[4];

        flagsToStrings[0] = new FlagToString(COINSPARK_ADDRESS_FLAG_ASSETS, "assets");
        flagsToStrings[1] = new FlagToString(COINSPARK_ADDRESS_FLAG_PAYMENT_REFS, "payment references");
        flagsToStrings[2] = new FlagToString(COINSPARK_ADDRESS_FLAG_TEXT_MESSAGES, "text messages");
        flagsToStrings[3] = new FlagToString(COINSPARK_ADDRESS_FLAG_FILE_MESSAGES, "file messages");

        StringBuilder sb = new StringBuilder();
        sb.append("COINSPARK ADDRESS\n")
                .append(String.format("  Bitcoin address: %s\n", bitcoinAddress))
                .append(String.format("    Address flags: %d", addressFlags)) ;

        boolean flagOutput = false;

        for (FlagToString flagsToString : flagsToStrings) {
            if ((addressFlags & flagsToString.flag) != 0) {
                sb.append(String.format("%s%s", flagOutput ? ", " : " [", flagsToString.string));
                flagOutput = true;
            }
        }

        sb.append(String.format("%s\n", flagOutput ? "]" : ""));

        sb.append(String.format("Payment reference: %d\n", paymentRef.ref))
            .append(String.format("END COINSPARK ADDRESS\n\n"));

        return  sb.toString();
    }

    /**
     * Returns true if all values in the address are in their permitted ranges, false otherwise.
     * 
     * @return Returns true if CoinSpark Address is valid
     */
    
    public boolean isValid()
    {
        if((bitcoinAddress == null) || (bitcoinAddress.length() == 0))
            return false;
        
        if((addressFlags & COINSPARK_ADDRESS_FLAG_MASK) != addressFlags)
            return false;
        
        if(paymentRef == null)
            return false;
        
        return paymentRef.isValid();
    }
    
    /**
     * Compares two CoinSpark addresses
     *  
     * @param address2 Address to compare with
     * @return Returns true if the two CoinSparkAddress structures are identical.
     */
    
    public boolean match(CoinSparkAddress address2)
    {
        return bitcoinAddress.toString().equals(address2.bitcoinAddress.toString()) &&
               this.addressFlags == address2.addressFlags &&
               this.paymentRef.match(address2.paymentRef);
    }

    /**
     * Encodes the fields in address CoinSpark address string.
     * 
     * @return String | null, Encoded CoinSpark address, null if we failed.
     */
    
    public String encode()
    {        
        try
        {
            if(!isValid())
                throw new CoinSparkExceptions.CannotEncode("Invalid CoinSpark address");
            
            CoinSparkBuffer buffer= new CoinSparkBuffer();
            
            int stringBase58[] = new int[1024];

            //  Build up extra data for address flags
            
            int addressFlagChars = 0;
            int testAddressFlags = addressFlags;

            while (testAddressFlags>0)
            {
                stringBase58[2+addressFlagChars]=testAddressFlags%58;
                testAddressFlags/=58;                                           // keep as integer
                addressFlagChars++;
            }

            //  Build up extra data for payment reference
            
            int paymentRefChars = 0;
            long testPaymentRef=paymentRef.ref;

            while (testPaymentRef > 0)
            {
                stringBase58[2+addressFlagChars+paymentRefChars] = (int)(testPaymentRef%58);
                testPaymentRef /=58;                                            // keep as integer
                paymentRefChars++;
            }

            //  Calculate total length required
            
            int extraDataChars = addressFlagChars+paymentRefChars;
            int bitcoinAddressLen = bitcoinAddress.length();
            int stringLen=bitcoinAddressLen+2+extraDataChars;
            
            stringBase58[1]=addressFlagChars*COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE+paymentRefChars;
            
            //  Convert the address itself
            
            for (int charIndex=0; charIndex<this.bitcoinAddress.length(); charIndex++)
            {
                int charValue = base58ToInteger((byte)bitcoinAddress.charAt(charIndex));
                if (charValue<0)
                    throw new CoinSparkExceptions.CannotEncode("Wrong address character");

                charValue+=COINSPARK_ADDRESS_CHAR_INCREMENT;
                
                if (extraDataChars>0)
                    charValue+=stringBase58[2+charIndex%extraDataChars];

                stringBase58[2+extraDataChars+charIndex]=charValue%58;
            }

            //  Obfuscate first half of address using second half to prevent common prefixes
            
            int halfLength=(stringLen+1)/2;
            for (int charIndex=1; charIndex<halfLength; charIndex++) // exclude first character
            {
                stringBase58[charIndex]=(stringBase58[charIndex]+stringBase58[stringLen-charIndex])%58;
            }
            
        //  Convert to base 58 and add prefix and terminator

            buffer.writeByte(COINSPARK_ADDRESS_PREFIX);                
            for (int charIndex=1; charIndex<stringLen; charIndex++)
            {
                buffer.writeByte((byte)(integerToBase58.charAt(stringBase58[charIndex])));
            }
            
            return buffer.toAscii();
        }
        
        catch (CoinSparkExceptions.CannotEncode ex)
        {
            System.out.print(ex.getMessage());
            return null;
        }
    }
    
    /**
     * Decodes the CoinSpark address string into the fields in address.
     * 
     * @param Address CoinSpark address to decode
     * @return true on success, false on failure
     */
    
    public boolean decode(String Address)
    {
        try
        {
            CoinSparkBuffer buffer= new CoinSparkBuffer(Address,false);
            int stringBase58[] = new int[1024];
            int stringLen=Address.length();
            
            //  Check for basic validity and get length of address flags

            if ( buffer.length() < 2)
                throw new CoinSparkExceptions.CannotDecode("Too Short");

            if (buffer.readByte()!=COINSPARK_ADDRESS_PREFIX)
                throw new CoinSparkExceptions.CannotDecode("Wrong Prefix");
                        
            //  Convert from base 58

            stringBase58[0]=COINSPARK_ADDRESS_PREFIX;
            for (int charIndex=1; charIndex<stringLen; charIndex++) 
            { // exclude first character
                int charValue=base58ToInteger(buffer.readByte());
                if (charValue<0)
                    throw new CoinSparkExceptions.CannotDecode("Invalid Address");
                stringBase58[charIndex]=charValue;
            }
            
            //  De-obfuscate first half of address using second half

            int halfLength=(buffer.length()+1)/2;
            for (int charIndex=1; charIndex<halfLength; charIndex++) // exclude first character
            {
                stringBase58[charIndex]=(stringBase58[charIndex]+58-stringBase58[stringLen-charIndex])%58;
            }
            
            int charValue = stringBase58[1];            
            int addressFlagChars = charValue/COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE; // keep as integer
            int paymentRefChars = charValue%COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE;
            int extraDataChars = addressFlagChars + paymentRefChars;

            if ( stringLen < (2+extraDataChars))
            {
                throw new CoinSparkExceptions.CannotDecode("Invalid Address");
            }

            //  Check we have sufficient length for the decoded address

            int bitcoinAddressLength = buffer.length() - 2 - extraDataChars;
            bitcoinAddress.setLength(bitcoinAddressLength);
            
            //  Read the extra data for address flags

            addressFlags = 0;
            long multiplier = 1;

            for (int charIndex=0; charIndex<addressFlagChars; charIndex++)
            {
                charValue = stringBase58[2+charIndex];
                if (charValue < 0)
                {
                    throw new CoinSparkExceptions.CannotDecode("Invalid Value (58 based < 0)");
                }
                addressFlags += multiplier * charValue;
                multiplier *= 58;
            }

            if ((addressFlags & COINSPARK_ADDRESS_FLAG_MASK) != addressFlags)
            {
                throw new CoinSparkExceptions.CannotDecode("Wrong Address Flag Mask");
            }
            
            //  Read the extra data for payment assetRef

            paymentRef = new CoinSparkPaymentRef(0);
            multiplier = 1;

            for (int charIndex=0; charIndex<paymentRefChars; charIndex++)
            {
                charValue = stringBase58[2+addressFlagChars+charIndex];
                if (charValue<0)
                {
                    throw new CoinSparkExceptions.CannotDecode("Invalid Value (58 based < 0)");
                }

                paymentRef.ref += multiplier * charValue;
                multiplier *= 58;
            }

            if(!paymentRef.isValid())
                throw new CoinSparkExceptions.CannotDecode("Wrong Payment Range");

            //  Convert the address itself

            for (int charIndex=0; charIndex<bitcoinAddressLength; charIndex++)
            {
                charValue=stringBase58[2+extraDataChars+charIndex];
                if (charValue<0)
                {
                    throw new CoinSparkExceptions.CannotDecode("Invalid Value (58 based < 0)");
                }
                
                charValue += 58*2-COINSPARK_ADDRESS_CHAR_INCREMENT;             // avoid worrying about the result of modulo on negative numbers in any language

                if (extraDataChars > 0)
                {
                    charValue -= stringBase58[2+charIndex%extraDataChars];
                }
                bitcoinAddress.setCharAt(charIndex, integerToBase58.charAt(charValue%58));
            }

            return true;
        }
        catch (CoinSparkExceptions.CannotDecode ex)
        {
            System.out.print(ex.getMessage());
            return false;
        }
    }

// Private variables/constants/functions   
    
    private static final byte COINSPARK_ADDRESS_PREFIX               = 's';
    private static final int  COINSPARK_ADDRESS_FLAG_CHARS_MULTIPLE  = 10;
    private static final int  COINSPARK_ADDRESS_CHAR_INCREMENT       = 13;
    
    private StringBuffer bitcoinAddress = null;
    private int addressFlags;
    private CoinSparkPaymentRef paymentRef;

    
    private static final StringBuffer integerToBase58 =
            new StringBuffer("123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz");


    private class FlagToString {
        public int flag;
        public String string;
        public FlagToString(int flag, String string)
        {
            this.flag = flag;
            this.string = string;
        }
        private FlagToString(){} // disabled
    };

}
