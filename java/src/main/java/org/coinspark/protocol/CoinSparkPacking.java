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

/**
 * Internal class with static functions needed for packing/unpacking transfer metadata
 */

public class CoinSparkPacking {
 
    static protected final int COINSPARK_UNSIGNED_BYTE_MAX       = 0xFF;
    static protected final int COINSPARK_UNSIGNED_2_BYTES_MAX    = 0xFFFF;
    static protected final int COINSPARK_UNSIGNED_3_BYTES_MAX    = 0xFFFFFF;
    static protected final long COINSPARK_UNSIGNED_4_BYTES_MAX   = 0xFFFFFFFFL;
    
    static protected final int COINSPARK_PACKING_GENESIS_MASK  = 0xC0;
    static protected final int COINSPARK_PACKING_GENESIS_PREV  = 0x00;

    static protected final int COINSPARK_PACKING_GENESIS_3_3_BYTES  = 0x40; // 3 bytes for block index, 3 for txn offset
    static protected final int COINSPARK_PACKING_GENESIS_3_4_BYTES  = 0x80; // 3 bytes for block index, 4 for txn offset
    static protected final int COINSPARK_PACKING_GENESIS_4_4_BYTES  = 0xC0; // 4 bytes for block index, 4 for txn offset

    static protected final byte COINSPARK_PACKING_INDICES_MASK       = 0x38;
    static protected final byte COINSPARK_PACKING_INDICES_0P_0P      = 0x00; // input 0 only or previous, output 0 only or previous
    static protected final byte COINSPARK_PACKING_INDICES_0P_1S      = 0x08; // input 0 only or previous, output 1 only or subsequent single
    static protected final byte COINSPARK_PACKING_INDICES_0P_ALL     = 0x10; // input 0 only or previous, all outputs
    static protected final byte COINSPARK_PACKING_INDICES_1S_0P      = 0x18; // input 1 only or subsequent single, output 0 only or previous
    static protected final byte COINSPARK_PACKING_INDICES_ALL_0P     = 0x20; // all inputs, output 0 only or previous
    static protected final byte COINSPARK_PACKING_INDICES_ALL_1S     = 0x28; // all inputs, output 1 only or subsequent single
    static protected final byte COINSPARK_PACKING_INDICES_ALL_ALL    = 0x30; // all inputs, all outputs
    static protected final byte COINSPARK_PACKING_INDICES_EXTEND     = 0x38; // use second byte for more extensive information

    static protected final byte COINSPARK_PACKING_EXTEND_INPUTS_SHIFT= 3;
    static protected final byte COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT=0;

    static protected final byte COINSPARK_PACKING_EXTEND_MASK        = 0x07;
    static protected final byte COINSPARK_PACKING_EXTEND_0P          = 0x00; // index 0 only or previous(transfers only)
    static protected final int  COINSPARK_PACKING_EXTEND_PUBLIC      = 0x00; // this is public (messages only)
    static protected final byte COINSPARK_PACKING_EXTEND_1S          = 0x01; // index 1 only or subsequent single (transfers only)
    static protected final byte COINSPARK_PACKING_EXTEND_0_1_BYTE    = 0x01; // // starting at 0, 1 byte for count (messages only)
    static protected final byte COINSPARK_PACKING_EXTEND_1_0_BYTE    = 0x02; // 1 byte for single index
    static protected final byte COINSPARK_PACKING_EXTEND_2_BYTES     = 0x03; // 2 bytes for single index
    static protected final byte COINSPARK_PACKING_EXTEND_1_1_BYTES   = 0x04; // 1 byte for first index, 1 byte for count
    static protected final byte COINSPARK_PACKING_EXTEND_2_1_BYTES   = 0x05; // 2 bytes for first index, 1 byte for count
    static protected final byte COINSPARK_PACKING_EXTEND_2_2_BYTES   = 0x06; // 2 bytes for first index, 2 bytes for count
    static protected final byte COINSPARK_PACKING_EXTEND_ALL         = 0x07; // all inputs|outputs

    static protected final byte COINSPARK_PACKING_QUANTITY_MASK      = 0x07;
    static protected final byte COINSPARK_PACKING_QUANTITY_1P        = 0x00; // quantity=1 or previous
    static protected final byte COINSPARK_PACKING_QUANTITY_1_BYTE    = 0x01;
    static protected final byte COINSPARK_PACKING_QUANTITY_2_BYTES   = 0x02;
    static protected final byte COINSPARK_PACKING_QUANTITY_3_BYTES   = 0x03;
    static protected final byte COINSPARK_PACKING_QUANTITY_4_BYTES   = 0x04;
    static protected final byte COINSPARK_PACKING_QUANTITY_6_BYTES   = 0x05;
    static protected final byte COINSPARK_PACKING_QUANTITY_FLOAT     = 0x06;
    static protected final byte COINSPARK_PACKING_QUANTITY_MAX       = 0x07; // transfer all quantity across
    
    private final static int maxPackingTypes = 9;//TBD
    protected enum PackingType {      // do not want to use ordinal. see Joshua Bloch, Effective Java (2nd ed), item 31
        _NONE(-1),
        _0P(0),
        _1S(1),
        _ALL(2),
        _1_0_BYTE(3),
        _0_1_BYTE(4),
        _2_BYTES(5),
        _1_1_BYTES(6),
        _2_1_BYTES(7),
        _2_2_BYTES(8),
        countPackingTypes(maxPackingTypes); //TBD

        private final int value;
        private PackingType(int value) {
            this.value = value;
        }

        protected int getValue() {
            return value;
        }
    }
    
    protected final static byte packingExtendMap[]={ // same order as above
            COINSPARK_PACKING_EXTEND_0P,
            COINSPARK_PACKING_EXTEND_1S,
            COINSPARK_PACKING_EXTEND_ALL,
            COINSPARK_PACKING_EXTEND_1_0_BYTE,
            COINSPARK_PACKING_EXTEND_0_1_BYTE,
            COINSPARK_PACKING_EXTEND_2_BYTES,
            COINSPARK_PACKING_EXTEND_1_1_BYTES,
            COINSPARK_PACKING_EXTEND_2_1_BYTES,
            COINSPARK_PACKING_EXTEND_2_2_BYTES,
    };

    protected static Byte encodePackingExtend(boolean [] packingOptions)
    {
        for (PackingType option :  PackingType.values())       
        {
            if (option != PackingType._NONE && option != PackingType.countPackingTypes && packingOptions[option.getValue()]) {
                return packingExtendMap[option.getValue()];
            }
        }
        return null;
    }
    
    
    protected static PackingType decodePackingExtend(byte packingExtend, boolean ForMessages)
    {
        PackingType packingType = PackingType._NONE;
        for (PackingType option :  PackingType.values()) {       
            if (option != PackingType._NONE && option != PackingType.countPackingTypes)
                if(option!=(ForMessages ? PackingType._1S : PackingType._0_1_BYTE))// no _1S for messages, no _0_1_BYTE for transfers
                    if (packingExtend==packingExtendMap[option.getValue()]) {                    
                        packingType=option;
                        return packingType;
                    }
        }

        return packingType;
    }
    
    
    protected class PackingByteCounts{
        protected int blockNumBytes;
        protected int txOffsetBytes;
        protected int txIDPrefixBytes;
        protected int firstInputBytes;
        protected int countInputsBytes;
        protected int firstOutputBytes;
        protected int countOutputsBytes;
        protected int quantityBytes;

        public PackingByteCounts()
        {
            //  Set default values for bytes for all fields to zero
            blockNumBytes = 0;
            txOffsetBytes = 0;
            txIDPrefixBytes = 0;
            firstInputBytes = 0;
            countInputsBytes = 0;
            firstOutputBytes = 0;
            countOutputsBytes = 0;
            quantityBytes = 0;
        }

        @Override
        public String toString()
        {
            return String.format(
                    "blockIndex %d, txnOffset %d, txnIDPrefix %d, firstInput %d, countInputs %d, firstOutput %d, countOutputs %d, quantity %d",
                    blockNumBytes, txOffsetBytes, txIDPrefixBytes,
                    firstInputBytes, countInputsBytes, firstOutputBytes, countOutputsBytes, quantityBytes);
        }
    };
    
    
    protected static CoinSparkIORange packingTypeToValues(PackingType packingType, CoinSparkIORange previousRange, int countInputOutputs)
    {
        CoinSparkIORange range=new CoinSparkIORange();
        switch (packingType)
        {
            case _0P:
                if (previousRange!= null) {
                    range.first=previousRange.first;
                    range.count=previousRange.count;
                } else {
                    range.first=0;
                    range.count=1;                    
                }
                break;

            case _1S:
                if (previousRange != null)
                    range.first=previousRange.first+previousRange.count;
                else
                    range.first=1;

                range.count=1;
                break;

            case _0_1_BYTE:
                range.first=0;
                break;
                
            case _1_0_BYTE:
            case _2_BYTES:
                range.count=1;
                break;

            case _ALL:
                range.first=0;
                range.count=countInputOutputs;
                break;

            default:                                                            // other packing types need to be read in
                break;
        }
        
        return range;
    }
    
    protected static int [] packingExtendAddByteCounts(int packingExtend,int firstBytes, int countBytes)
    {
        int [] result=new int[3];
        
        result[0]=0;                                                            // Reserved for packing
        result[1]=firstBytes;
        result[2]=countBytes;
        
        switch (packingExtend)
        {
            case COINSPARK_PACKING_EXTEND_0_1_BYTE:
                result[2]=1;
                break;
		
            case COINSPARK_PACKING_EXTEND_1_0_BYTE:
                result[1] = 1;
                break;

            case COINSPARK_PACKING_EXTEND_2_BYTES:
                result[1] = 2;
                break;

            case COINSPARK_PACKING_EXTEND_1_1_BYTES:
                result[1] = 1;
                result[2] = 1;
                break;

            case COINSPARK_PACKING_EXTEND_2_1_BYTES:
                result[1] = 2;
                result[2] = 1;
                break;

            case COINSPARK_PACKING_EXTEND_2_2_BYTES:
                result[1] = 2;
                result[2] = 2;
                break;
        }
        
        return result;
    }
    
    protected static PackingByteCounts packingToByteCounts(byte packing, byte packingExtend)
    {
        //  Set default values for bytes for all fields to zero
        
        PackingByteCounts counts=new CoinSparkPacking().new PackingByteCounts();
        
        //  Packing for genesis reference
        
        switch (packing & COINSPARK_PACKING_GENESIS_MASK)
        {
            case COINSPARK_PACKING_GENESIS_3_3_BYTES:
                counts.blockNumBytes = 3;
                counts.txOffsetBytes = 3;
                counts.txIDPrefixBytes = CoinSparkAssetRef.COINSPARK_ASSETREF_TXID_PREFIX_LEN;
                break;

            case COINSPARK_PACKING_GENESIS_3_4_BYTES:
                counts.blockNumBytes = 3;
                counts.txOffsetBytes = 4;
                counts.txIDPrefixBytes = CoinSparkAssetRef.COINSPARK_ASSETREF_TXID_PREFIX_LEN;
                break;

            case COINSPARK_PACKING_GENESIS_4_4_BYTES:
                counts.blockNumBytes = 4;
                counts.txOffsetBytes = 4;
                counts.txIDPrefixBytes = CoinSparkAssetRef.COINSPARK_ASSETREF_TXID_PREFIX_LEN;
                break;
        }

        //  Packing for input and output indices  (relevant for extended indices only)

        if ((packing & COINSPARK_PACKING_INDICES_MASK) == COINSPARK_PACKING_INDICES_EXTEND) { // we're using extended indices

            int countsBytes[];
            
            //  Input indices
            countsBytes=packingExtendAddByteCounts(((packingExtend >> COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK),
                counts.firstInputBytes,counts.countInputsBytes);
            counts.firstInputBytes=countsBytes[1];
            counts.countInputsBytes=countsBytes[2];
            
            //  Output indices
            countsBytes=packingExtendAddByteCounts(((packingExtend >> COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK),
                counts.firstOutputBytes,counts.countOutputsBytes);
            counts.firstOutputBytes=countsBytes[1];
            counts.countOutputsBytes=countsBytes[2];
            
/*                
            switch ((packingExtend >> COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK)
            {
                case COINSPARK_PACKING_EXTEND_1_0_BYTE:
                    counts.firstInputBytes = 1;
                    break;

                case COINSPARK_PACKING_EXTEND_2_BYTES:
                    counts.firstInputBytes = 2;
                    break;

                case COINSPARK_PACKING_EXTEND_1_1_BYTES:
                    counts.firstInputBytes = 1;
                    counts.countInputsBytes = 1;
                    break;

                case COINSPARK_PACKING_EXTEND_2_1_BYTES:
                    counts.firstInputBytes = 2;
                    counts.countInputsBytes = 1;
                    break;

                case COINSPARK_PACKING_EXTEND_2_2_BYTES:
                    counts.firstInputBytes = 2;
                    counts.countInputsBytes = 2;
                    break;
            }

            //  Output indices

            switch ((packingExtend >> COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) & COINSPARK_PACKING_EXTEND_MASK)
            {
                case COINSPARK_PACKING_EXTEND_1_0_BYTE:
                    counts.firstOutputBytes = 1;
                    break;

                case COINSPARK_PACKING_EXTEND_2_BYTES:
                    counts.firstOutputBytes = 2;
                    break;

                case COINSPARK_PACKING_EXTEND_1_1_BYTES:
                    counts.firstOutputBytes = 1;
                    counts.countOutputsBytes = 1;
                    break;

                case COINSPARK_PACKING_EXTEND_2_1_BYTES:
                    counts.firstOutputBytes = 2;
                    counts.countOutputsBytes = 1;
                    break;

                case COINSPARK_PACKING_EXTEND_2_2_BYTES:
                    counts.firstOutputBytes = 2;
                    counts.countOutputsBytes = 2;
                    break;
            }

*/
        }
        //  Packing for quantity

        switch (packing & COINSPARK_PACKING_QUANTITY_MASK)
        {
            case COINSPARK_PACKING_QUANTITY_1_BYTE:
                counts.quantityBytes = 1;
                break;

            case COINSPARK_PACKING_QUANTITY_2_BYTES:
                counts.quantityBytes = 2;
                break;

            case COINSPARK_PACKING_QUANTITY_3_BYTES:
                counts.quantityBytes = 3;
                break;

            case COINSPARK_PACKING_QUANTITY_4_BYTES:
                counts.quantityBytes = 4;
                break;

            case COINSPARK_PACKING_QUANTITY_6_BYTES:
                counts.quantityBytes = 6;
                break;

            case COINSPARK_PACKING_QUANTITY_FLOAT:
                counts.quantityBytes = CoinSparkTransfer.COINSPARK_TRANSFER_QTY_FLOAT_LENGTH;
                break;
        }
        
        return counts;
    }
    
    protected static boolean [] getPackingOptions(CoinSparkIORange previousRange, CoinSparkIORange range, int countInputOutputs,boolean ForMessages)
    {
        boolean [] packingOptions=new boolean[maxPackingTypes];
        
        boolean firstZero, firstByte, first2Bytes, countOne, countByte;

        firstZero=(range.first == 0);
        firstByte=(range.first <= COINSPARK_UNSIGNED_BYTE_MAX);
        first2Bytes=(range.first <= COINSPARK_UNSIGNED_2_BYTES_MAX);
        countOne=(range.count == 1);
        countByte=(range.count <= COINSPARK_UNSIGNED_BYTE_MAX);

        if(ForMessages)
        {
            packingOptions[PackingType._0P.getValue()]=false;
            packingOptions[PackingType._1S.getValue()]=false;
            packingOptions[PackingType._0_1_BYTE.getValue()]=firstZero && countByte;

        }
        else
        {
            if (previousRange != null) {
                packingOptions[PackingType._0P.getValue()]=(range.first==previousRange.first) &&
                        (range.count == previousRange.count);
                packingOptions[PackingType._1S.getValue()]=(range.first == (previousRange.first + previousRange.count)) && countOne;

            } else {
                packingOptions[PackingType._0P.getValue()]=firstZero && countOne;
                packingOptions[PackingType._1S.getValue()]=(range.first==1) && countOne;
            }
            packingOptions[PackingType._0_1_BYTE.getValue()]=false;
        }

        packingOptions[PackingType._1_0_BYTE.getValue()]=firstByte && countOne;
        packingOptions[PackingType._2_BYTES.getValue()]=first2Bytes && countOne;
        packingOptions[PackingType._1_1_BYTES.getValue()]=firstByte && countByte;
        packingOptions[PackingType._2_1_BYTES.getValue()]=first2Bytes && countByte;
        packingOptions[PackingType._2_2_BYTES.getValue()]=first2Bytes && (range.count <= COINSPARK_UNSIGNED_2_BYTES_MAX);
        packingOptions[PackingType._ALL.getValue()]=firstZero && (range.count >= countInputOutputs);
        
        return packingOptions;
    }
    
}
