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
 * CoinSpark class for Input/Output range used in CoinSpark transfers.
 */

public class CoinSparkIORange {
    
    /**
     * First I/O
     */

    public int first;
    
    /**
     * Number of I/Os in range
     */
    
    public int count;

    /**
     * CoinSpark class for Input/Output range used in CoinSpark transfers.
     */
    
    public CoinSparkIORange()
    {
        first = 0;
        count = 0;
    }

    /**
     * CoinSpark class for Input/Output range used in CoinSpark transfers.
     * 
     * @param First to set
     * @param Count to set
     */
    
    public CoinSparkIORange(int First, int Count)
    {
        first = First;
        count = Count;
    }
    
    @Override
    public CoinSparkIORange clone() throws CloneNotSupportedException
    {
        CoinSparkIORange copy=new CoinSparkIORange();
        copy.first=first;
        copy.count=count;
        return copy;
    }
        
    /**
     * Returns true if all values in the I/O range are in their permitted ranges, false otherwise.
     * 
     * @return true if IORange structure is valid
     */    
    
    public boolean isValid()
    {
        if( (first < 0) || (first > COINSPARK_IO_INDEX_MAX) )
        {
            return false;
        }
        if( (count < 0) || (count > COINSPARK_IO_INDEX_MAX) )
        {
            return false;
        }
        return true;
    }
    
    /**
     * Returns true if the two CoinSparkIORange structures are the same. 
     * 
     * @param range2 CoinSparkIORange to compare with
     * @return true if two CoinSparkIORange match, false otherwise
     */
    

    public boolean match(CoinSparkIORange range2)
    {
        if(count != range2.count)
            return false;
        
        if(first != range2.first)
            return false;
        
        return true;
    }
    
    protected static CoinSparkIORange [] normalizeIORanges(CoinSparkIORange [] inRanges)
    {
        int countRanges=inRanges.length;
        if(countRanges == 0)
        {
            return inRanges;
        }

        boolean [] rangeUsed = new boolean[countRanges];
        CoinSparkIORange [] outRanges = new CoinSparkIORange[countRanges];
        int countRemoved=0;
        int lastRangeEnd=-1;
        
        for(int i=0;i<countRanges;i++)
        {
            rangeUsed[i]=false;
        }
        
        for (int orderIndex=0; orderIndex<countRanges; orderIndex++) 
        {
            int lowestRangeFirst=0;
            int lowestRangeIndex=-1;
		
            for (int rangeIndex=0; rangeIndex<countRanges; rangeIndex++)
            {                
		if (!rangeUsed[rangeIndex])
                {                    
                    if ( (lowestRangeIndex==-1) || (inRanges[rangeIndex].first<lowestRangeFirst) ) 
                    {
                        lowestRangeFirst=inRanges[rangeIndex].first;
                        lowestRangeIndex=rangeIndex;
                    }
                }
            }
		
            if ((orderIndex>0) && (inRanges[lowestRangeIndex].first<=lastRangeEnd)) 
            {                                                                   // we can combine two adjacent ranges
                countRemoved++;
                int thisRangeEnd=inRanges[lowestRangeIndex].first+inRanges[lowestRangeIndex].count;
                outRanges[orderIndex-countRemoved].count=Math.max(lastRangeEnd, thisRangeEnd)-outRanges[orderIndex-countRemoved].first;

            } 
            else
            {
                outRanges[orderIndex-countRemoved]=inRanges[lowestRangeIndex];
            }
            
            lastRangeEnd=outRanges[orderIndex-countRemoved].first+outRanges[orderIndex-countRemoved].count;
            rangeUsed[lowestRangeIndex]=true;
        }
        
        return inRanges;
    }
    
    
    protected static final int COINSPARK_IO_INDEX_MAX  = 65535;

}
