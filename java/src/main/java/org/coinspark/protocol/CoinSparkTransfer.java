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

import java.util.Arrays;

/**
 * Class for managing individual asset transfer metadata
 */


public class CoinSparkTransfer extends CoinSparkBase{
    
    
    /**
     * Class for managing individual asset transfer metadata
     */
    
    public CoinSparkTransfer()
    {
        clear();
    }
    
    /**
     * Set all fields in transfer to their default/zero values, which are not necessarily valid.
     */
    
    
    public final void clear()
    {
        assetRef = new CoinSparkAssetRef();
        inputs = new CoinSparkIORange();
        outputs = new CoinSparkIORange();
        qtyPerOutput = 0;
    }
    
    /**
     * Returns asset reference
     * 
     * @return asset reference
     */
    
    public CoinSparkAssetRef getAssetRef() {
        return assetRef;
    }

    /**
     * Sets asset reference
     * 
     * @param assetRef to set
     */
    
    public void setAssetRef(CoinSparkAssetRef assetRef) {
        this.assetRef = assetRef;
    }

    /**
     * Returns inputs range
     * 
     * @return inputs range
     */
    
    public CoinSparkIORange getInputs() {
        return inputs;
    }

    /**
     * Sets inputs range
     * 
     * @param inputs to set
     */
    
    public void setInputs(CoinSparkIORange inputs) {
        this.inputs = inputs;
    }

    /**
     * Returns outputs range
     * 
     * @return outputs range
     */
    
    public CoinSparkIORange getOutputs() {
        return outputs;
    }

    /**
     * Sets output range 
     * @param outputs to set
     */
    
    public void setOutputs(CoinSparkIORange outputs) {
        this.outputs = outputs;
    }

    /**
     * Returns Quantity per output
     * 
     * @return  Quantity per output
     */
    
    public long getQtyPerOutput() {
        return qtyPerOutput;
    }

    /**
     * Sest quantity per output
     * 
     * @param qtyPerOutput 
     */
    
    public void setQtyPerOutput(long qtyPerOutput) {
        this.qtyPerOutput = qtyPerOutput;
    }
    
    @Override
    public CoinSparkTransfer clone() throws CloneNotSupportedException
    {
        CoinSparkTransfer copy=new CoinSparkTransfer();
        
        copy.assetRef=assetRef.clone();
        copy.inputs=inputs.clone();
        copy.outputs=outputs.clone();
        copy.qtyPerOutput=qtyPerOutput;
                
        return copy;
    }

    /**
     * Returns true if the two CoinSparkTransfer structures are identical.
     * 
     * @param transfer2 CoinSparkTransfer to compare to 
     * @return Returns true if the two CoinSparkTransfer structures are identical.
     */
    
    public boolean match(CoinSparkTransfer transfer2)
    {
        boolean partialMatch = (this.inputs.first == transfer2.inputs.first) &&
                               (this.inputs.count == transfer2.inputs.count) &&
                               (this.outputs.first == transfer2.outputs.first);

        if (this.assetRef.getBlockNum() == COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
            return (transfer2.assetRef.getBlockNum() == COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) && partialMatch;

        else
            return
                    this.assetRef.match(transfer2.assetRef) && partialMatch &&
                            (this.getOutputs().count==transfer2.getOutputs().count) &&
                            (this.getQtyPerOutput() == transfer2.getQtyPerOutput());
    }

    /**
     * Returns true if all values in the transfer are in their permitted ranges, false otherwise.
     * 
     * @return true if genesis structure is valid
     */
    
    public boolean isValid()
    {
        if(!(assetRef.isValid() && inputs.isValid() && outputs.isValid()))
        {
            return false;
        }
        
        if( (qtyPerOutput < 0) || (qtyPerOutput > CoinSparkAssetQty.COINSPARK_ASSET_QTY_MAX) )
        {
            return false;
        }
        
        return true;
    }
    
    @Override
    public String toString()
    {
        return CoinSparkTransfer.toStringInner(this, true);
    }
    
    
// Private variables/constants/functions       
    
    
    protected static final int COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE      = -1; // magic number for a default route
    
    protected static final int COINSPARK_TRANSFER_QTY_FLOAT_LENGTH             = 2;
    protected static final short COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX     = 1000;
    protected static final short COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX     = 11;
    protected static final int COINSPARK_TRANSFER_QTY_FLOAT_MASK               = 0x3FFF;
    protected static final int COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE  = 1001;



    protected CoinSparkAssetRef assetRef;
    protected CoinSparkIORange inputs;
    protected CoinSparkIORange outputs;
    protected long qtyPerOutput;

    
    

    protected static String toStringInner(CoinSparkTransfer transfer, boolean headers)
    {
        StringBuilder sb = new StringBuilder();
        if (headers)
            sb.append("COINSPARK TRANSFER\n");

        boolean isDefaultRoute=(transfer.assetRef.getBlockNum() == COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);

        if (isDefaultRoute)
        {
            sb.append("      Default route:\n");
        }
        else
        {
            sb.append(transfer.assetRef.toStringInner(false));

            sb.append(String.format("    Asset reference: %s\n", transfer.assetRef.encode()));
        }

        if (transfer.getInputs().count > 0)
        {
            if (transfer.getInputs().count > 1)
                sb.append(String.format("             Inputs: %d - %d (count %d)",
                        transfer.getInputs().first,
                        transfer.getInputs().first + transfer.getInputs().count - 1,
                        transfer.getInputs().count));
            else
                sb.append(String.format("              Input: %d", transfer.getInputs().first));
        }
        else
            sb.append("           Inputs: none");

        sb.append(String.format(" (small endian hex: first %s count %s)\n",
                unsignedToSmallEndianHex(transfer.getInputs().first, 2),
                unsignedToSmallEndianHex(transfer.getInputs().count, 2))); //oooo why biginteger

        if (transfer.getOutputs().count>0)
        {
            if (transfer.getOutputs().count > 1 && !isDefaultRoute)
                sb.append(String.format("            Outputs: %d - %d (count %d)",
                        transfer.getOutputs().first,
                        transfer.getOutputs().first + transfer.getOutputs().count - 1,
                        transfer.getOutputs().count));
            else
                sb.append(String.format("             Output: %d", transfer.getOutputs().first));
        } else
            sb.append("          Outputs: none");

        sb.append(String.format(" (small endian hex: first %s count %s)\n",
                unsignedToSmallEndianHex(transfer.getOutputs().first, 2),
                unsignedToSmallEndianHex(transfer.getOutputs().count, 2)));

        if (!isDefaultRoute)
        {
            sb.append(String.format("     Qty per output: %d (small endian hex %s", transfer.getQtyPerOutput(),
                    unsignedToSmallEndianHex(transfer.getQtyPerOutput(), 8)));
            CoinSparkAssetQty qtyEncodedFloat=new CoinSparkAssetQty(transfer.qtyPerOutput, 0, COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX,COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX);
            if(qtyEncodedFloat.value == transfer.qtyPerOutput) 
            {
                long encodeQuantity = new CoinSparkAssetQty(
                        (qtyEncodedFloat.exponent*COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE + qtyEncodedFloat.mantissa) & COINSPARK_TRANSFER_QTY_FLOAT_MASK).value;

                sb.append(String.format(", as float %s", unsignedToSmallEndianHex(
                        encodeQuantity, COINSPARK_TRANSFER_QTY_FLOAT_LENGTH)));
            }

            sb.append(")\n");
        }
        if (headers)
            sb.append("END COINSPARK TRANSFER\n\n");

        return sb.toString();
    }
    
    protected boolean encode(CoinSparkBuffer buffer, CoinSparkTransfer previousTransfer, int countInputs, int countOutputs)
    {
        boolean[] inputPackingOptions;
        boolean[] outputPackingOptions;
        byte packing,packingExtend;
        Byte packingExtendInput, packingExtendOutput;
        
        
        CoinSparkGenesis tempGenesis = new CoinSparkGenesis();
        long encodeQuantity;

        try
        {
            if(!isValid())
            {
                throw new CoinSparkExceptions.CannotEncode("Invalid transfer");
            }
            
            boolean isDefaultRoute=(this.assetRef.getBlockNum() == COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);

            packing=0;
            packingExtend=0;
            
            //  Packing for genesis reference
            
            if (isDefaultRoute)
            {
                if (previousTransfer != null &&
                   (previousTransfer.assetRef.getBlockNum() != COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE))
                    throw new CoinSparkExceptions.CannotEncode("No default route");
                    // default route transfers have to come at the start

                packing |= CoinSparkPacking.COINSPARK_PACKING_GENESIS_PREV;

            }
            else
            {
                if (previousTransfer != null && this.assetRef.match(previousTransfer.assetRef))
                    packing |= CoinSparkPacking.COINSPARK_PACKING_GENESIS_PREV;

                else if (this.assetRef.getBlockNum() <= CoinSparkPacking.COINSPARK_UNSIGNED_3_BYTES_MAX)
                {
                    if (this.assetRef.getTxOffset() <= CoinSparkPacking.COINSPARK_UNSIGNED_3_BYTES_MAX)
                        packing|= CoinSparkPacking.COINSPARK_PACKING_GENESIS_3_3_BYTES;
                    else if (this.assetRef.getTxOffset() <= CoinSparkPacking.COINSPARK_UNSIGNED_4_BYTES_MAX)
                        packing|= CoinSparkPacking.COINSPARK_PACKING_GENESIS_3_4_BYTES;
                    else
                        throw new CoinSparkExceptions.CannotEncode("Wrong block number");
                }

                else if ((this.assetRef.getBlockNum() <= CoinSparkPacking.COINSPARK_UNSIGNED_4_BYTES_MAX) &&
                         (this.assetRef.getTxOffset() <= CoinSparkPacking.COINSPARK_UNSIGNED_4_BYTES_MAX))
                    packing|= CoinSparkPacking.COINSPARK_PACKING_GENESIS_4_4_BYTES;

                else
                    throw new CoinSparkExceptions.CannotEncode("Block number out of range");
            }

            //  Packing for input and output indices
            inputPackingOptions = CoinSparkPacking.getPackingOptions(previousTransfer != null ?
                    previousTransfer.inputs : null, this.inputs, countInputs,false);
            outputPackingOptions = CoinSparkPacking.getPackingOptions(previousTransfer != null ?
                    previousTransfer.outputs : null, this.outputs, countOutputs,false);

            if (inputPackingOptions[CoinSparkPacking.PackingType._0P.getValue()] &&
                outputPackingOptions[CoinSparkPacking.PackingType._0P.getValue()])
                packing|= CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_0P;

            else if (inputPackingOptions[CoinSparkPacking.PackingType._0P.getValue()] &&
                outputPackingOptions[CoinSparkPacking.PackingType._1S.getValue()])
                packing|= CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_1S;

            else if (inputPackingOptions[CoinSparkPacking.PackingType._0P.getValue()] &&
                    outputPackingOptions[CoinSparkPacking.PackingType._ALL.getValue()])
                packing|= CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_ALL;

            else if (inputPackingOptions[CoinSparkPacking.PackingType._1S.getValue()] &&
                outputPackingOptions[CoinSparkPacking.PackingType._0P.getValue()])
                packing|= CoinSparkPacking.COINSPARK_PACKING_INDICES_1S_0P;

            else if (inputPackingOptions[CoinSparkPacking.PackingType._ALL.getValue()] &&
                outputPackingOptions[CoinSparkPacking.PackingType._0P.getValue()])
                packing|= CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_0P;

            else if (inputPackingOptions[CoinSparkPacking.PackingType._ALL.getValue()] &&
                outputPackingOptions[CoinSparkPacking.PackingType._1S.getValue()])
                packing|= CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_1S;

            else if (inputPackingOptions[CoinSparkPacking.PackingType._ALL.getValue()] &&
                outputPackingOptions[CoinSparkPacking.PackingType._ALL.getValue()])
                packing|= CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_ALL;

            else {                                                              // we need the second (extended) packing byte
                packing|= CoinSparkPacking.COINSPARK_PACKING_INDICES_EXTEND;

            if( (packingExtendInput = CoinSparkPacking.encodePackingExtend(inputPackingOptions)) == null)
                throw new CoinSparkExceptions.CannotEncode("Input packing error");

            if( (packingExtendOutput = CoinSparkPacking.encodePackingExtend(outputPackingOptions)) == null)
                throw new CoinSparkExceptions.CannotEncode("Input packing error");
                
            packingExtend = (byte)((packingExtendInput << CoinSparkPacking.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) |
                                       (packingExtendOutput << CoinSparkPacking.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT));  
            }

            //  Packing for quantity
            
            encodeQuantity=this.qtyPerOutput;
            CoinSparkAssetQty qtyEncodedFloat=new CoinSparkAssetQty(this.qtyPerOutput, 0, COINSPARK_TRANSFER_QTY_FLOAT_MANTISSA_MAX,COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MAX);
            
            if (this.qtyPerOutput==(previousTransfer != null ? previousTransfer.qtyPerOutput : 1))
                packing|= CoinSparkPacking.COINSPARK_PACKING_QUANTITY_1P;

            else if (this.qtyPerOutput >= CoinSparkAssetQty.COINSPARK_ASSET_QTY_MAX)
                packing|= CoinSparkPacking.COINSPARK_PACKING_QUANTITY_MAX;

            else if (this.qtyPerOutput <= CoinSparkPacking.COINSPARK_UNSIGNED_BYTE_MAX)
                packing|= CoinSparkPacking.COINSPARK_PACKING_QUANTITY_1_BYTE;

            else if (this.qtyPerOutput <= CoinSparkPacking.COINSPARK_UNSIGNED_2_BYTES_MAX)
                packing|= CoinSparkPacking.COINSPARK_PACKING_QUANTITY_2_BYTES;

            else if (qtyEncodedFloat.value==this.qtyPerOutput)
            {
                packing |= CoinSparkPacking.COINSPARK_PACKING_QUANTITY_FLOAT;
                encodeQuantity = (qtyEncodedFloat.exponent*COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE +
                        qtyEncodedFloat.mantissa)&COINSPARK_TRANSFER_QTY_FLOAT_MASK;
            }

            else if (this.qtyPerOutput <= CoinSparkPacking.COINSPARK_UNSIGNED_3_BYTES_MAX)
                packing|= CoinSparkPacking.COINSPARK_PACKING_QUANTITY_3_BYTES;

            else if (this.qtyPerOutput <= CoinSparkPacking.COINSPARK_UNSIGNED_4_BYTES_MAX)
                packing|= CoinSparkPacking.COINSPARK_PACKING_QUANTITY_4_BYTES;

            else
                packing|= CoinSparkPacking.COINSPARK_PACKING_QUANTITY_6_BYTES;


            //  Write out the actual data

            CoinSparkPacking.PackingByteCounts counts = CoinSparkPacking.transferPackingToByteCounts(packing, packingExtend);  

            buffer.writeByte(packing);

            if ((packing & CoinSparkPacking.COINSPARK_PACKING_INDICES_MASK) == CoinSparkPacking.COINSPARK_PACKING_INDICES_EXTEND)
            {
                buffer.writeByte(packingExtend);
            }

            buffer.writeLong(assetRef.getBlockNum(),counts.blockNumBytes);
            buffer.writeLong(assetRef.getTxOffset(),counts.txOffsetBytes);
            buffer.writeBytes(assetRef.getTxIDPrefix(), counts.txIDPrefixBytes);
            buffer.writeInt(inputs.first, counts.firstInputBytes);
            buffer.writeInt(inputs.count, counts.countInputsBytes);
            buffer.writeInt(outputs.first, counts.firstOutputBytes);
            buffer.writeInt(outputs.count, counts.countOutputsBytes);
            buffer.writeLong(encodeQuantity,counts.quantityBytes);
            

        }
        catch (Exception ex)
        {
            System.out.print(ex.getMessage());
            return false;
        }
        
        return true;
    }


    protected boolean decode(CoinSparkBuffer buffer, CoinSparkTransfer previousTransfer, int countInputs, int countOutputs)
    {
        byte packing, packingExtend = 0;
        CoinSparkPacking.PackingType inputPackingType = CoinSparkPacking.PackingType._NONE;
        CoinSparkPacking.PackingType outputPackingType = CoinSparkPacking.PackingType._NONE;
        
        try
        {
            //  Extract packing
            
            if(buffer.canRead(1))
            {
                packing=buffer.readByte();
            }
            else
                throw new CoinSparkExceptions.CannotDecode("Cannot read packing");


            // Packing for genesis reference
            
            switch (packing & CoinSparkPacking.COINSPARK_PACKING_GENESIS_MASK)
            {
                case CoinSparkPacking.COINSPARK_PACKING_GENESIS_PREV:
                    if (previousTransfer != null)
                        this.assetRef =previousTransfer.assetRef;

                    else {                                                      // it's for a default route
                        this.assetRef.setBlockNum(COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE);
                        this.assetRef.setTxOffset(0);
                        Arrays.fill(this.assetRef.getTxIDPrefix(), (byte)0);
                    }
                    break;
            }

            //  Packing for input and output indices

            if ((packing & CoinSparkPacking.COINSPARK_PACKING_INDICES_MASK) == CoinSparkPacking.COINSPARK_PACKING_INDICES_EXTEND)
            {
                // we're using second packing metadata byte
                
                if(buffer.canRead(1))
                {
                    packingExtend=buffer.readByte();
                }
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read packing extend");

                inputPackingType=CoinSparkPacking.decodePackingExtend((byte) ((packingExtend >> CoinSparkPacking.COINSPARK_PACKING_EXTEND_INPUTS_SHIFT) &
                                CoinSparkPacking.COINSPARK_PACKING_EXTEND_MASK),false);

                if (inputPackingType == CoinSparkPacking.PackingType._NONE)
                    throw new CoinSparkExceptions.CannotDecode("Wrong packing type");
                
                outputPackingType = CoinSparkPacking.decodePackingExtend((byte) ((packingExtend >> CoinSparkPacking.COINSPARK_PACKING_EXTEND_OUTPUTS_SHIFT) &
                                CoinSparkPacking.COINSPARK_PACKING_EXTEND_MASK),false);
                if (outputPackingType == CoinSparkPacking.PackingType._NONE)
                    throw new CoinSparkExceptions.CannotDecode("No packing type");
                

            } else {                                                            // not using second packing metadata byte

                switch (packing & CoinSparkPacking.COINSPARK_PACKING_INDICES_MASK) // input packing
                {
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_0P:
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_1S:
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_ALL:
                        inputPackingType= CoinSparkPacking.PackingType._0P;
                        break;

                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_1S_0P:
                        inputPackingType= CoinSparkPacking.PackingType._1S;
                        break;

                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_0P:
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_1S:
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_ALL:
                        inputPackingType= CoinSparkPacking.PackingType._ALL;
                        break;
                }

                switch (packing & CoinSparkPacking.COINSPARK_PACKING_INDICES_MASK) // output packing
                {
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_0P:
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_1S_0P:
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_0P:
                        outputPackingType= CoinSparkPacking.PackingType._0P;
                        break;

                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_1S:
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_1S:
                        outputPackingType= CoinSparkPacking.PackingType._1S;
                        break;

                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_0P_ALL:
                    case CoinSparkPacking.COINSPARK_PACKING_INDICES_ALL_ALL:
                        outputPackingType= CoinSparkPacking.PackingType._ALL;
                        break;
                }
            }

            inputs=CoinSparkPacking.packingTypeToValues(inputPackingType, previousTransfer != null ? previousTransfer.inputs : null, countInputs);
            outputs=CoinSparkPacking.packingTypeToValues(outputPackingType, previousTransfer != null ? previousTransfer.outputs : null, countOutputs);

            //  Read in the fields as appropriate
            
            CoinSparkPacking.PackingByteCounts counts = CoinSparkPacking.transferPackingToByteCounts(packing, packingExtend);  
            
            long[] resLong = new long[1];
            if (counts.blockNumBytes>0)
            {
                if(buffer.canRead(counts.blockNumBytes))
                    assetRef.setBlockNum(buffer.readLong(counts.blockNumBytes));
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read block number");
            }

            if (counts.txOffsetBytes>0)
            {
                if(buffer.canRead(counts.txOffsetBytes))
                    assetRef.setTxOffset(buffer.readLong(counts.txOffsetBytes));
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read txn offset");
            }
            
            if (counts.txIDPrefixBytes>0)
            {
                if(buffer.canRead(counts.txIDPrefixBytes))
                    assetRef.setTxIDPrefix(buffer.readBytes(counts.txIDPrefixBytes));
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read txn prefix");
            }
            
            if (counts.firstInputBytes>0)
            {
                if(buffer.canRead(counts.firstInputBytes))
                    inputs.first=buffer.readInt(counts.firstInputBytes);
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read input first");
            }

            if (counts.countInputsBytes>0)
            {
                if(buffer.canRead(counts.countInputsBytes))
                    inputs.count=buffer.readInt(counts.countInputsBytes);
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read input count");
            }

            if (counts.firstOutputBytes>0)
            {
                if(buffer.canRead(counts.firstOutputBytes))
                    outputs.first=buffer.readInt(counts.firstOutputBytes);
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read output first");
            }

            if (counts.countOutputsBytes>0)
            {
                if(buffer.canRead(counts.countOutputsBytes))
                    outputs.count=buffer.readInt(counts.countOutputsBytes);
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read output count");
            }

            long decodeQuantity=0;
            if (counts.quantityBytes>0)
            {
                if(buffer.canRead(counts.quantityBytes))
                    decodeQuantity = buffer.readLong(counts.quantityBytes);
                else
                    throw new CoinSparkExceptions.CannotDecode("Cannot read quantity");
            }
            
            
            //  Finish up reading in quantity

            switch (packing & CoinSparkPacking.COINSPARK_PACKING_QUANTITY_MASK)
            {
                case CoinSparkPacking.COINSPARK_PACKING_QUANTITY_1P:
                    if (previousTransfer != null)
                        this.qtyPerOutput = previousTransfer.qtyPerOutput;
                    else
                        this.qtyPerOutput = 1;
                    break;

                case CoinSparkPacking.COINSPARK_PACKING_QUANTITY_MAX:
                    this.qtyPerOutput = CoinSparkAssetQty.COINSPARK_ASSET_QTY_MAX;
                    break;

                case CoinSparkPacking.COINSPARK_PACKING_QUANTITY_FLOAT:
                    decodeQuantity &= COINSPARK_TRANSFER_QTY_FLOAT_MASK;
                    
                    this.qtyPerOutput = new CoinSparkAssetQty(
                            (int)(decodeQuantity%COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE),
                            (int)(decodeQuantity/COINSPARK_TRANSFER_QTY_FLOAT_EXPONENT_MULTIPLE)).value;
                    break;

                default:
                    this.qtyPerOutput=decodeQuantity;
                    break;
            }
        }
        catch (Exception ex)
        {
            System.out.print(ex.getMessage());
            return false;
        }
        
        return true;
    }

}

