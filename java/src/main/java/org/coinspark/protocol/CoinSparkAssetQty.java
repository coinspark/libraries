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

/**
 * CoinSpark asset quantity.
 */

public class CoinSparkAssetQty {
    
    /**
     * CoinSpark asset quantity long (64-bit) value
     */
    
    protected long value = 0;                  
    
    /**
     * CoinSpark asset quantity mantissa
     */
    
    protected int mantissa = 0;
    
    /**
     * CoinSpark asset quantity exponent
     */
    
    protected int exponent = 0;
    
    /**
     * CoinSpark asset quantity.
     * 
     * @param Value Long (64-bit) value to set
     */
    
    protected CoinSparkAssetQty(long Value)
    {
        value = Value;
    }

    /**
     * CoinSpark asset quantity.
     * 
     * @param Mantissa mantissa to set
     * @param Exponent exponent to set
     */
    
    protected CoinSparkAssetQty(int Mantissa, int Exponent)
    {
        construct(Mantissa, Exponent);
    }
   
    
    @Override
    protected CoinSparkAssetQty clone() throws CloneNotSupportedException
    {
        CoinSparkAssetQty copy=new CoinSparkAssetQty(value);
        copy.exponent=exponent;
        copy.mantissa=mantissa;
        return copy;
    }
    
    
    
    
// Private variables/constants/functions       
    
    protected static final long COINSPARK_ASSET_QTY_MAX  = 100000000000000L;
    
    private void construct(int Mantissa, int Exponent)
    {
        mantissa=Mantissa;
        exponent=Exponent;
        
        value = mantissa;

        int e=exponent;
        for (; e>0; e--)
        {
            value*=10;        
        }
    }    
    
    protected CoinSparkAssetQty(long Quantity, int Rounding, int MantissaMax, short ExponentMax)
    {
        long roundOffset;

        if (Rounding<0)
            roundOffset=0;
        else if (Rounding>0)
            roundOffset=9;
        else
            roundOffset=4;

        int Exponent = 0;

        while (Quantity > MantissaMax)
        {
            Quantity = (Quantity+roundOffset)/10;
            Exponent++;
        }

        int Mantissa = (int)Quantity;
        if (Exponent > ExponentMax)
            Exponent = ExponentMax;

        construct(Mantissa, Exponent);
    }
    
}
