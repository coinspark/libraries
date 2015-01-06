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

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * CoinSparkTransferList class for managing list of asset transfer metadata
 */

public class CoinSparkTransferList extends CoinSparkBase{
    

    
    /**
     * CoinSparkTransferList class for managing list of asset transfer metadata
     */

    public CoinSparkTransferList()
    {
        maxTransfers = MAX_TRANSFERS_DEFAULT;
        transfersList = new CoinSparkTransfer[maxTransfers];
    }

    /**
     * CoinSparkTransferList class for managing list of asset transfer metadata
     * @param MaxTransfers maximal number of transfers
     */
    
    public CoinSparkTransferList(int MaxTransfers)
    {
        maxTransfers = MaxTransfers;
        transfersList = new CoinSparkTransfer[maxTransfers];
    }

    /**
     * Returns real number of transfer in the list
     * @return number of transfers in the list
     */
    
    public int count() {
        return countTransfers;
    }

    /**
     * Return individual transfer
     * @param i -transfer id (0 based)
     * @return individual transfer
     */
    
    public CoinSparkTransfer getTransfer(int i)
    {
        if (i < transfersList.length)
            return transfersList[i];
        return null;
    }
    
    @Override
    public String toString()
    {
        int[] ordering = new int[1024];
        if (countTransfers > ordering.length)
            return null;

        transfersGroupOrdering(this.transfersList, ordering, countTransfers);

        StringBuilder sb = new StringBuilder();
        sb.append("COINSPARK TRANSFERS\n");

        for (int index=0; index < countTransfers; index++)
        {
            if (transfersList[index] == null)
                break;

            if (index>0)
                sb.append("\n");

            sb.append(CoinSparkTransfer.toStringInner(transfersList[index], false));
        }

        sb.append("END COINSPARK TRANSFERS\n\n");
        return sb.toString();
    }
    
    /**
     * If strict is true then the ordering in the two arrays must be identical. If strict is false
     * then it is enough if each list is equivalent, i.e. the same transfers in the same order for each asset reference.
     * 
     * @param transfers2 transfer list to compare to 
     * @param strict strinct flag
     * @return Returns true if the two arrays of transfers in this and transfers2 are the same.
     */
    
    public boolean match(CoinSparkTransferList transfers2,  boolean strict)
    {
        int[] ordering1 = new int[1024];
        int[] ordering2 = new int[1024];
        
        if(countTransfers != transfers2.countTransfers)
        {
            return false;
        }
        
        if (strict)
        {
            for (int transferIndex=0; transferIndex<countTransfers; transferIndex++) {
                if (!this.transfersList[transferIndex].match(transfers2.transfersList[transferIndex]))
                    return false;
            }
        }
        else
        {
            if (countTransfers>ordering1.length)
                return false;

            transfersGroupOrdering(this.transfersList, ordering1, countTransfers);
            transfersGroupOrdering(transfers2.transfersList, ordering2, countTransfers);
            for(int i = 0; i < countTransfers; i++)
            {
                if (!transfersList[i].match(transfers2.transfersList[i]))
                {
                    return false;
                }
            }
        }
        return true;
    }
    
    @Override
    public CoinSparkTransferList clone() throws CloneNotSupportedException
    {
        CoinSparkTransferList clone = new CoinSparkTransferList(maxTransfers);
        clone.maxTransfers = maxTransfers;
        clone.countTransfers = this.countTransfers;
        clone.transfersList = new CoinSparkTransfer[maxTransfers];
        for (int i = 0; i < maxTransfers; i++) {
            clone.transfersList[i] = this.transfersList[i].clone();
        }

        return clone;
    }

    /**
     * Adds transfer to transfer list
     * 
     * @param i id in list
     * @param transfer to set
     */
    
    public void setTransfer(int i, CoinSparkTransfer transfer) 
    {
        if (maxTransfers > i)
        {
            transfersList[i] = transfer;
            if(i+1>countTransfers)
            {
                countTransfers=i+1; 
           }
        }
    }
    
    /**
     * Encodes the transfer list into metadata. 
     * 
     * @param countInputs number of inputs in transaction
     * @param countOutputs number of outputs in transaction
     * @param metadataMaxLen maximal size of metadata
     * @return String | null Encoded transfer list as hexadecimal, null if we failed.
     */
    
    public String encodeToHex(int countInputs, int countOutputs,int metadataMaxLen)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer();
        if(!encode(buffer,countInputs,countOutputs,metadataMaxLen))
        {
            return null;
        }
        
        return buffer.toHex();
    }
    
    
    /**
     * Encodes the transfer list into metadata. 
     * 
     * @param countInputs number of inputs in transaction
     * @param countOutputs number of outputs in transaction
     * @param metadataMaxLen maximal size of metadata
     * @return byte [] | null Encoded transfer list as raw data, null if we failed.
     */
    
    public byte [] encode(int countInputs, int countOutputs,int metadataMaxLen)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer();
        
        if(!encode(buffer,countInputs,countOutputs,metadataMaxLen))
        {
            return null;
        }
        
        return buffer.toBytes();
    }
    
    /**
     * Decodes the metadata into transfer list.
     * 
     * @param metadata Metadata to decode as hexadecimal
     * @param countInputs number of inputs in transaction
     * @param countOutputs number of outputs in transaction
     * @return true on success, false on failure
     */
    
    
    public boolean decode(String metadata, int countInputs, int countOutputs)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata, true);
        return decode(buffer,countInputs,countOutputs);
    }
    
    /**
     * Decodes the metadata into transfer list.
     * 
     * @param metadata Metadata to decode as raw data
     * @param countInputs number of inputs in transaction
     * @param countOutputs number of outputs in transaction
     * @return true on success, false on failure
     */
    
    public boolean decode(byte [] metadata, int countInputs, int countOutputs)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata);
        return decode(buffer,countInputs,countOutputs);
    }
    
    /**
     * Decodes the metadata into transfer list.
     * 
     * @param metadata Metadata to decode as hexadecimal
     * @param countInputs number of inputs in transaction
     * @param countOutputs number of outputs in transaction
     * @return number of transfers
     */
    
    public int decodeCount(String metadata, int countInputs, int countOutputs)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata, true);
        return decodeCount(buffer);
    }
    
    /**
     * Decodes the metadata into transfer list.
     * 
     * @param metadata Metadata to decode as raw data
     * @param countInputs number of inputs in transaction
     * @param countOutputs number of outputs in transaction
     * @return number of transfers
     */
    
    public int decodeCount(byte [] metadata, int countInputs, int countOutputs)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata);
        return decodeCount(buffer);
    }

    /**
     * Use CoinSparkScriptIsRegular() to pass an array of bools in outputsRegular for whether each output script is regular.
     * Pass the number of transaction inputs and outputs in countInputs and countOutputs respectively.
     * @param countInputs number of inputs in transaction
     * @param outputsSatoshis Pass the number of bitcoin satoshis in each output in outputsSatoshis (array size countOutputs).
     * @param outputsRegular pass array of booleans for whether each output script is regular
     * @return Returns the minimum transaction fee (in bitcoin satoshis) required to make the set of transfers (array size countTransfers) valid.
    */
    
    public long calcMinFee(int countInputs, long[] outputsSatoshis, boolean[] outputsRegular) {
        
        if(outputsSatoshis.length != outputsRegular.length)
        {
            return COINSPARK_SATOSHI_QTY_MAX;
        }
        
        int countOutputs=Math.min(outputsSatoshis.length, outputsRegular.length);
                
        int outputIndex, lastOutputIndex;

        int transfersToCover = 0;

        for (int transferIndex = 0; transferIndex < countTransfers; transferIndex++) {
            if (
                (this.transfersList[transferIndex].assetRef.getBlockNum() !=
                        CoinSparkTransfer.COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE) && // don't count default routes
                (this.transfersList[transferIndex].inputs.count > 0) &&
                (this.transfersList[transferIndex].inputs.first < countInputs) // only count if at least one valid input index
                )
            {
                outputIndex = Math.max(this.transfersList[transferIndex].outputs.first, 0);
                lastOutputIndex = Math.min(this.transfersList[transferIndex].outputs.first +
                        this.transfersList[transferIndex].outputs.count, countOutputs)-1;

                for (; outputIndex<=lastOutputIndex; outputIndex++) {
                    if (outputsRegular[outputIndex])
                        transfersToCover++;
                }
            }
        }
        long temp = getMinFeeBasis(outputsSatoshis, outputsRegular);
        temp *= transfersToCover;
        return temp;
    }

    /**
     * For the asset specified by assetRef and genesis, and list of transfers (size countTransfers), applies those transfers
     * to move units of that asset from inputBalances (size countInputs) to outputBalances.
     * Only transfers whose assetRef matches the function's assetRef parameter will be applied (apart from default routes).
     * Use CoinSparkScriptIsRegular() to pass an array of bools in outputsRegular for whether each output script is regular.
     * ** Call this if the transaction DOES HAVE a sufficient fee to make the list of transfers valid **
     * 
     * @param reference Asset reference
     * @param genesis Genesis object corresponding to asset reference
     * @param inputBalances Input balances
     * @param outputsRegular pass array of booleans for whether each output script is regular
     * @return Output balances
     */
    
    public long[] apply(CoinSparkAssetRef reference, CoinSparkGenesis genesis,
               long[] inputBalances,boolean[] outputsRegular)
    {
        return applyInner(reference, genesis,  inputBalances, outputsRegular);
    }

    
    /**
     * For the asset specified by assetRef and genesis, move units of that asset from inputBalances (size countInputs) to
     * outputBalances, applying the default behavior only (all assets goes to last regular output).
     * ** Call this if the transaction DOES NOT HAVE a sufficient fee to make the list of transfers valid **
     * @param assetRef Asset reference
     * @param genesis Genesis object corresponding to asset reference
     * @param inputBalances Input balances
     * @param outputsRegular pass array of booleans for whether each output script is regular
     * @return Output balances
     */
    public long[]  applyNone(CoinSparkAssetRef assetRef, CoinSparkGenesis genesis,
                                     long[] inputBalances,boolean[] outputsRegular)
    {
        return new CoinSparkTransferList().applyInner(assetRef, genesis,inputBalances,outputsRegular);
    }

    /**
     * For the list of transfers (size countTransfers) on a transaction with countInputs inputs, calculate
     * the array of bools in outputsDefault where each entry indicates whether that
     * output might receive some assets due to default routes.
     *
     * @param countInputs number of inputs in transaction
     * @param outputsRegular pass array of booleans for whether each output script is regular
     * @return array of booleans indicating where each entry indicates whether that output might receive some assets due to default routes.
    */
    
    public boolean [] defaultOutputs(int countInputs, boolean[] outputsRegular)
    {
        int countOutputs=outputsRegular.length;
        boolean [] outputsDefault=new boolean [countOutputs];
        int outputIndex;
        for (outputIndex = 0; outputIndex < countOutputs; outputIndex++) {
            outputsDefault[outputIndex] = false;
        }

        int[] inputDefaultOutput = getDefaultRouteMap(countInputs, outputsRegular);

        for (int inputIndex=0; inputIndex<countInputs; inputIndex++) {
            outputIndex=inputDefaultOutput[inputIndex];

            if (outputIndex<countOutputs)
                outputsDefault[outputIndex] = true;
        }
        return outputsDefault;
    }

    
    
// Private variables/constants/functions       
    
    
    private static final int MAX_TRANSFERS_DEFAULT = 40;

    private CoinSparkTransfer[] transfersList;
    private int maxTransfers;
    private int countTransfers;


    private boolean encode(CoinSparkBuffer buffer, int countInputs, int countOutputs,int metadataMaxLen)
    {
        int[] ordering = new int[1024];

        if (countTransfers > ordering.length)
            return false; // too many for statically sized array

        buffer.writeString(COINSPARK_METADATA_IDENTIFIER);
        buffer.writeByte(COINSPARK_TRANSFERS_PREFIX);
        

        //  Encode each transfer, grouping by asset reference, but preserving original order otherwise

        transfersGroupOrdering(this.transfersList, ordering, countTransfers);

        CoinSparkTransfer previousTransfer = null;

        for (int transferIndex = 0; transferIndex<countTransfers; transferIndex++)
        {
            if(!transfersList[ordering[transferIndex]].encode(buffer, previousTransfer, countInputs, countOutputs))
            {
                return false;
            }

            previousTransfer = transfersList[ordering[transferIndex]];
        }

        if(buffer.length()>metadataMaxLen)
        {
            return false;
        }
        
        return true;
    }
    
    
    private boolean decode(CoinSparkBuffer buffer, int countInputs, int countOutputs)
    {
        CoinSparkTransfer transfer, previousTransfer = null;
        long transferBytesUsed = 0;

        if(!buffer.locateRange(COINSPARK_TRANSFERS_PREFIX))
            return false;
        
        //  Iterate over list

        try {
            countTransfers=0;
            transfer = new CoinSparkTransfer();
            while (buffer.availableForRead() > 0)
            {            
                if(transfer.decode(buffer, countTransfers != 0 ? previousTransfer : null, countInputs, countOutputs))
                {
                    if (countTransfers < maxTransfers)
                        transfersList[countTransfers]=transfer.clone(); // copy across if still space

                    countTransfers++;
                    previousTransfer=transfer.clone();
                }
                else
                    return false;

            }
        } catch (CloneNotSupportedException ex) {
            Logger.getLogger(CoinSparkTransferList.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return true;
    }
    
    
    private int decodeCount(CoinSparkBuffer buffer)
    {
        if(decode(buffer, CoinSparkIORange.COINSPARK_IO_INDEX_MAX, CoinSparkIORange.COINSPARK_IO_INDEX_MAX))
        {
            return countTransfers;
        }
        return 0;
    }
    
    private long[] applyInner(CoinSparkAssetRef reference, CoinSparkGenesis genesis, 
               long[] inputBalances, boolean[] outputsRegular)
    {
        int countInputs=inputBalances.length;
        int countOutputs=outputsRegular.length;

        long[] outputBalances=new long[countOutputs];
        int transferIndex;
        int inputIndex, outputIndex, lastInputIndex, lastOutputIndex;
        long transferRemaining;
        long transferQuantity;

        //  Copy all input quantities and zero output quantities

        long[] inputsRemaining = new long[countInputs];
        for (inputIndex=0; inputIndex<countInputs; inputIndex++) {
            inputsRemaining[inputIndex]=inputBalances[inputIndex];
        }

        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            outputBalances[outputIndex] = 0;
        }

        //  Perform explicit this (i.e. not default routes)

        for (transferIndex=0; transferIndex < countTransfers; transferIndex++)
        {
            if (reference.match(this.transfersList[transferIndex].getAssetRef())) {
                inputIndex = Math.max(this.transfersList[transferIndex].getInputs().first, 0);
                outputIndex = Math.max(this.transfersList[transferIndex].getOutputs().first, 0);

                lastInputIndex = Math.min(inputIndex + this.transfersList[transferIndex].getInputs().count, countInputs) - 1;
                lastOutputIndex = Math.min(outputIndex + this.transfersList[transferIndex].getOutputs().count, countOutputs) - 1;

                for (; outputIndex<=lastOutputIndex; outputIndex++) {
                    if (outputsRegular[outputIndex]) {
                        transferRemaining= this.transfersList[transferIndex].getQtyPerOutput();

                        while (inputIndex<=lastInputIndex) {
                            transferQuantity = Math.min(transferRemaining, inputsRemaining[inputIndex]);

                            if (transferQuantity>0)                       // skip all this if nothing is to be transferred (branch not really necessary)
                            {
                                inputsRemaining[inputIndex] = inputsRemaining[inputIndex] - transferQuantity;
                                transferRemaining = transferRemaining - transferQuantity;

                                transferQuantity = Math.min(transferQuantity,
                                        CoinSparkAssetQty.COINSPARK_ASSET_QTY_MAX - outputBalances[outputIndex]); // prevent overflow
                                outputBalances[outputIndex] = (outputBalances[outputIndex] + transferQuantity);
                            }

                            if (transferRemaining>0)
                                inputIndex++;                                   // move to next input since this one is drained
                            else
                                break;                                          // stop if we have nothing left to transfer
                        }
                    }
                }
            }
        }

        //  Apply payment charges to all quantities not routed by default

        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            if (outputsRegular[outputIndex] && (outputBalances[outputIndex]>0) )
                outputBalances[outputIndex] = genesis.calcNet(outputBalances[outputIndex]);
        }

        //  Send remaining quantities to default outputs

        int[] inputDefaultOutput = getDefaultRouteMap(countInputs, outputsRegular);

        for (inputIndex=0; inputIndex<countInputs; inputIndex++) {
            outputIndex=inputDefaultOutput[inputIndex];
            if (outputIndex<countOutputs)                                       // could be out of range if there are no regular outputs
                outputBalances[outputIndex] += inputsRemaining[inputIndex];
        }
        
        return outputBalances;
    }

    private int[] getDefaultRouteMap(int countInputs, boolean[] outputsRegular)
    {
        
        int countOutputs=outputsRegular.length;
        int lastInputIndex, inputIndex, outputIndex;

        int[] inputDefaultOutput = new int[countInputs];

        //  Default to last output for all inputs

        int lastRegularOutput = getLastRegularOutput(outputsRegular);  // can be countOutputs if no regular ones found
        for (inputIndex=0; inputIndex<countInputs; inputIndex++) {
            inputDefaultOutput[inputIndex]=lastRegularOutput;
        }

        //  Apply any default route transfers in reverse order (since early ones take precedence)

        for (int transferIndex=countTransfers-1; transferIndex>=0; transferIndex--) {
            if (transfersList[transferIndex].assetRef.getBlockNum() ==
                    CoinSparkTransfer.COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
            {
                outputIndex = transfersList[transferIndex].outputs.first; // outputs.count is not relevant

                if ( (outputIndex>=0) && (outputIndex<countOutputs) ) {
                    inputIndex = Math.max(transfersList[transferIndex].inputs.first, 0);
                    lastInputIndex = Math.min(inputIndex + transfersList[transferIndex].inputs.count - 1, countInputs - 1);

                    for (; inputIndex<=lastInputIndex; inputIndex++) {
                        inputDefaultOutput[inputIndex]=outputIndex;
                    }
                }
            }
        }

        return inputDefaultOutput;
    }

    private static int[] transfersGroupOrdering(CoinSparkTransfer[] transfers, int[] ordering, int countTransfers)
    {
        int orderIndex, transferIndex, bestTransferIndex, transferScore, bestTransferScore;

        boolean[] transferUsed = new boolean[countTransfers];

        for (transferIndex=0; transferIndex<countTransfers; transferIndex++) {
            transferUsed[transferIndex] = false;
        }

        for (orderIndex=0; orderIndex<countTransfers; orderIndex++) {
            bestTransferScore=0;
            bestTransferIndex=-1;

            for (transferIndex=0; transferIndex<countTransfers; transferIndex++) {
                if (!transferUsed[transferIndex])
                {
                    if (transfers[transferIndex].assetRef.getBlockNum() ==
                            CoinSparkTransfer.COINSPARK_TRANSFER_BLOCK_NUM_DEFAULT_ROUTE)
                        transferScore=3;                                        // top priority to default routes, which must be first in the encoded list

                    else if ((orderIndex>0) && transfers[ordering[orderIndex-1]].assetRef.match(transfers[transferIndex].assetRef))
                        transferScore=2;                                        // then next best is one which has same asset reference as previous
                    else
                        transferScore=1;                                        // otherwise any will do

                    if (transferScore>bestTransferScore) {                      // if it's clearly the best, take it
                        bestTransferScore=transferScore;
                        bestTransferIndex=transferIndex;

                    } else if (transferScore==bestTransferScore)                // otherwise give priority to "lower" asset references
                        if (transfers[transferIndex].assetRef.compare(transfers[bestTransferIndex].assetRef)<0)
                            bestTransferIndex=transferIndex;
                }
            }

            ordering[orderIndex]=bestTransferIndex;
            transferUsed[bestTransferIndex] = true;
        }

        return ordering;
    }
    
}
