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
 * CoinSparkGenesis class for managing asset genesis metadata
 */

public class CoinSparkGenesis extends CoinSparkBase{

    
    public static final int COINSPARK_GENESIS_DOMAIN_NAME_MAX_LEN    = 32;
    public static final int COINSPARK_GENESIS_PAGE_PATH_MAX_LEN      = 24;
    
//	Public functions

    /**
     * CoinSparkGenesis class for managing asset genesis metadata
     */
    
    public CoinSparkGenesis()
    {
        clear();
    }
    
    /**
     * Set all fields in genesis to their default/zero values, which are not necessarily valid.
     */
    
    public final void clear()
    {
        qtyExponent = 0;
        qtyMantissa = 0;
        chargeFlatMantissa = 0;
        chargeFlatExponent = 0;
        chargeBasisPoints = 0;                                                  // one hundredths of a percent
        assetHash = new byte[COINSPARK_GENESIS_HASH_MAX_LEN];
        assetHashLen = 0;                                                       // number of bytes in assetHash that are valid for comparison
        domainName="";
        pagePath="";
        useHttps=false;
        usePrefix=true;
    }
    
    /**
     * Returns Charge basis points 
     * 
     * @return Charge basis points 
     */
    
    public short getChargeBasisPoints() {
        return chargeBasisPoints;
    }

    /**
     * sets Charge basis points 
     * 
     * @param ChargeBasisPoints to set
     */
    
    public void setChargeBasisPoints(short ChargeBasisPoints) {
        chargeBasisPoints = ChargeBasisPoints;
    }

    /**
     * Returns Charge flat mantissa
     * 
     * @return Charge flat mantissa
     */
    
    public short getChargeFlatMantissa() {

        return chargeFlatMantissa;
    }

    /**
     * Sets Charge flat mantissa.
     * 
     * @param ChargeFlatMantissa to set
     */
    
    public void setChargeFlatMantissa(short ChargeFlatMantissa) {
        chargeFlatMantissa = ChargeFlatMantissa;
    }

    /**
     * Returns Charge flat exponent
     * 
     * @return Charge flat exponent
     */
    
    public short getChargeFlatExponent() {
        return chargeFlatExponent;
    }

    /**
     * Sets Charge flat exponent
     * 
     * @param ChargeFlatExponent to set
     */
    
    public void setChargeFlatExponent(short ChargeFlatExponent) {
        chargeFlatExponent = ChargeFlatExponent;
    }

    /**
     * Returns Quantity exponent
     * 
     * @return Quantity exponent
     */
    
    public short getQtyExponent() {

        return qtyExponent;
    }

    /**
     * Sets Quantity exponent
     * 
     * @param QtyExponent to set
     */
    
    public void setQtyExponent(short QtyExponent) {
        qtyExponent = QtyExponent;
    }
    
    /**
     * Returns Quantity mantissa
     * 
     * @return Quantity mantissa
     */

    public short getQtyMantissa() {
        return qtyMantissa;
    }

    /**
     * Sets Quantity mantissa
     * 
     * @param QtyMantissa to set
     */
    
    public void setQtyMantissa(short QtyMantissa) {
        qtyMantissa = QtyMantissa;
    }

    /**
     * Returns Domain Name
     * 
     * @return Domain Name
     */

    public String getDomainName() {
        return domainName;
    }

    /**
     * Sets DomainName
     * 
     * @param DomainName to set
     */
    
    public void setDomainName(String DomainName) {
        domainName = DomainName;
    }

    /**
     * Returns Page Path
     * 
     * @return Page Path
     */

    public String getPagePath() {
        return pagePath;
    }

    /**
     * Sets Page Path
     * 
     * @param PagePath to set
     */
    
    public void setPagePath(String PagePath) {
        pagePath = PagePath;
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
     * Returns Asset hash
     * 
     * @return Asset hash
     */

    public byte [] getAssetHash() {
        return assetHash;
    }
    
    /**
     * Returns Asset hash length (to be) encoded
     * 
     * @return Asset hash length
     */

    public int getAssetHashLen() {
        return assetHashLen;
    }
    
    /**
     * Sets asset hash.
     * 
     * @param AssetHash to set
     */
    
    public void setAssetHash(byte [] AssetHash) {
        assetHash=Arrays.copyOf(AssetHash, AssetHash.length);
    }
    
    /**
     * Sets asset hash.
     * 
     * @param AssetHashLen to set
     */
    
    public void setAssetHashLen(int AssetHashLen) {
        assetHashLen=AssetHashLen;
    }
    
    @Override
    public String toString()
    {
        long quantity = getQty();
        int quantityEncoded = (this.qtyExponent * COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE + this.qtyMantissa) &
                                COINSPARK_GENESIS_QTY_MASK;

        long chargeFlat = getChargeFlat();
        int chargeFlatEncoded = this.chargeFlatExponent*COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE + this.chargeFlatMantissa;

        StringBuilder sb = new StringBuilder();
        
        CoinSparkBuffer assetWebPageBuffer=new CoinSparkBuffer();
        String encodedWebPage="";
        
        CoinSparkAssetWebPage assetWebPage=new CoinSparkAssetWebPage(domainName, pagePath, useHttps, usePrefix);
        if(assetWebPage.encode(assetWebPageBuffer))
        {
            encodedWebPage=assetWebPageBuffer.toHex();
        }
        
        sb.append("COINSPARK GENESIS\n")
                .append(String.format("   Quantity mantissa: %d\n", this.qtyMantissa))
                .append(String.format("   Quantity exponent: %d\n", this.qtyExponent))
                .append(String.format("    Quantity encoded: %d (small endian hex %s)\n", quantityEncoded,
                        unsignedToSmallEndianHex(quantityEncoded, 2)))
                .append(String.format("      Quantity value: %d\n", quantity))
                .append(String.format("Flat charge mantissa: %d\n", this.chargeFlatMantissa))
                .append(String.format("Flat charge exponent: %d\n", this.chargeFlatExponent))
                .append(String.format(" Flat charge encoded: %d (small endian hex %s)\n", chargeFlatEncoded,
                        unsignedToSmallEndianHex(chargeFlatEncoded, COINSPARK_GENESIS_CHARGE_FLAT_LENGTH)))
                .append(String.format("   Flat charge value: %d\n", chargeFlat))
                .append(String.format(" Basis points charge: %d (hex %s)\n", this.chargeBasisPoints,
                        unsignedToSmallEndianHex(this.chargeBasisPoints, COINSPARK_GENESIS_CHARGE_BPS_LENGTH)))
                .append(String.format("           Asset URL: %s://%s/%s%s/ (length %d+%d encoded %s length %d)\n",
                        assetWebPage.useHttps ? "https" : "http", assetWebPage.domainName, 
                        assetWebPage.usePrefix ? "coinspark/" : "",
                        (assetWebPage.path.length()>0) ? assetWebPage.path : "[spent-txid]",
                        assetWebPage.domainName.length(),assetWebPage.path.length(),
                        encodedWebPage,assetWebPage.encodedLen()))
                .append(String.format("          Asset hash: "))
                .append(byteToHex(Arrays.copyOf(assetHash, assetHashLen)))
                .append(String.format(" (length %d)\n", this.assetHashLen))
                .append(String.format("END COINSPARK GENESIS\n\n"));

        return  sb.toString();
    }

    /**
     * Returns true if the two CoinSparkGenesis structures are the same. If strict is true then
     * the qtyMantissa, qtyExponent, chargeFlatMantissa and chargeFlatExponent fields must be identical.
     * If strict is false then it is enough if each pair just represents the same final quantity.
     *
     * @param genesis2 CoinSparkGenesis to compare with
     * @param strict Strict comparison flag
     * @return true if two CoinSparkGenesis match, false otherwise
     */
    
    
    public boolean match(CoinSparkGenesis genesis2, boolean strict)
    {
        boolean floatQuantitiesMatch;
        int hashCompareLen = Math.min(this.assetHashLen, genesis2.assetHashLen);
        hashCompareLen = Math.min(hashCompareLen, COINSPARK_GENESIS_HASH_MAX_LEN);

        CoinSparkAssetWebPage assetWebPage=new CoinSparkAssetWebPage(domainName, pagePath, useHttps, usePrefix);
        CoinSparkAssetWebPage assetWebPage2=new CoinSparkAssetWebPage(genesis2.getDomainName(), 
                genesis2.getPagePath(), genesis2.getUseHttps(), genesis2.getUsePrefix());
        
        if (strict)
            floatQuantitiesMatch=(this.qtyMantissa == genesis2.qtyMantissa) &&
                    (this.qtyExponent == genesis2.qtyExponent) &&
                    (this.chargeFlatMantissa == genesis2.chargeFlatMantissa) &&
                    (this.chargeFlatExponent == genesis2.chargeFlatExponent);
        else
            floatQuantitiesMatch=(this.getQty() == genesis2.getQty()) &&
                    (this.getChargeFlat()==genesis2.getChargeFlat());

        return
                floatQuantitiesMatch && (this.chargeBasisPoints == genesis2.chargeBasisPoints) &&
                        assetWebPage.match(assetWebPage2) && 
                        (memcmp(this.assetHash, genesis2.assetHash, hashCompareLen) == 0)
                ;
    }
    
    /**
     * Returns true if all values in the genesis are in their permitted ranges, false otherwise.
     * 
     * @return true if genesis structure is valid
     */
    
    public boolean isValid()
    {
        if ( (qtyMantissa<COINSPARK_GENESIS_QTY_MANTISSA_MIN) || (qtyMantissa>COINSPARK_GENESIS_QTY_MANTISSA_MAX) )
            return false;

        if ( (qtyExponent<COINSPARK_GENESIS_QTY_EXPONENT_MIN) || (qtyExponent>COINSPARK_GENESIS_QTY_EXPONENT_MAX) )
            return false;

        if ( (chargeFlatExponent<COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN) ||
             (chargeFlatExponent>COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) )
            return false;

        if (chargeFlatMantissa<COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN)
            return false;

        if (chargeFlatMantissa > ((chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) ?
                COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX : COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX))
            return false;

        if ( (chargeBasisPoints<COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN) ||
             (chargeBasisPoints>COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX) )
            return false;
        
        CoinSparkAssetWebPage assetWebPage=new CoinSparkAssetWebPage(domainName, pagePath, useHttps, usePrefix);
        if (!assetWebPage.isValid())
            return false;
        
        if ( (assetHashLen<COINSPARK_GENESIS_HASH_MIN_LEN) || (assetHashLen>COINSPARK_GENESIS_HASH_MAX_LEN) )
            return false;

        return true;
    }

    /**
     * Returns the number of units denoted by the genesis qtyMantissa and qtyExponent fields.
     *
     * @return the number of units denoted by the genesis qtyMantissa and qtyExponent fields.
     */
    
    public long getQty()
    {
        return new CoinSparkAssetQty(this.qtyMantissa, this.qtyExponent).value;
    }

    /**
     * Sets the qtyMantissa and qtyExponent fields in genesis to be as close to desiredQty as possible.
     * Set rounding to [-1, 0, 1] for rounding [down, closest, up] respectively.
     * Returns the quantity that was actually encoded, via CoinSparkGenesisGetQty().
     *
     * @param desiredQty desired quantity
     * @param rounding [-1, 0, 1] for rounding [down, closest, up] respectively.
     * @return  the quantity that was actually encoded
     */
    
    public long setQty(long desiredQty, int rounding)
    {
        CoinSparkAssetQty qty=new CoinSparkAssetQty(desiredQty, rounding,
                COINSPARK_GENESIS_QTY_MANTISSA_MAX, COINSPARK_GENESIS_QTY_EXPONENT_MAX);
        
        qtyMantissa = (short)qty.mantissa;
        qtyExponent = (short)qty.exponent;        
        
        return getQty();
    }

    /**
     * Returns the number of units denoted by the genesis chargeFlatMantissa and chargeFlatExponent fields.
     * 
     * @return the number of units denoted by the genesis chargeFlatMantissa and chargeFlatExponent fields.
     */
    
    public long getChargeFlat()
    {
        return new CoinSparkAssetQty(this.chargeFlatMantissa, this.chargeFlatExponent).value;
    }

    /**
     * Sets the chargeFlatMantissa and chargeFlatExponent fields in genesis to be as close to desiredChargeFlat as possible.
     * Set rounding to [-1, 0, 1] for rounding [down, closest, up] respectively.
     * Returns the quantity that was actually encoded, via CoinSparkGenesisGetChargeFlat().
     *
     * @param desiredChargeFlat desired quantity
     * @param rounding [-1, 0, 1] for rounding [down, closest, up] respectively.
     * @return the quantity that was actually encoded
     */
    
    public long setChargeFlat(long desiredChargeFlat, int rounding)
    {
        CoinSparkAssetQty qty=new CoinSparkAssetQty(desiredChargeFlat, rounding,
                COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX, COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX);
        
        chargeFlatMantissa = (short)qty.mantissa;
        chargeFlatExponent = (short)qty.exponent;
        
        if (chargeFlatExponent == COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX)
            chargeFlatMantissa  = (short)Math.min(chargeFlatMantissa, COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX);
        
        return getChargeFlat();
    }

    /**
     * Calculates the payment charge specified by genesis for sending the raw quantity qtyGross.
     *
     * @param qtyGross quantity to send
     * @return the payment charge 
     */
    
    public long calcCharge(long qtyGross)
    {
        long charge = getChargeFlat() +(qtyGross*this.chargeBasisPoints+5000)/10000; // rounds to nearest
        if (charge > qtyGross)// can't charge more than the final amount
            charge = qtyGross;
        
        return charge;
    }

    /**
     * Calculates the quantity that will be received after the payment charge specified by genesis is applied to qtyGross.
     *
     * @param qtyGross quantity to send
     * @return the quantity that will be received
     */
    
    public long calcNet(long qtyGross)
    {
        return qtyGross - calcCharge(qtyGross);
    }

    /**
     * Calculates the quantity that should be sent so that, after the payment charge specified by genesis
     * is applied, the recipient will receive qtyNet units.
     *
     * @param qtyNet quantity to receive
     * @return the quantity that should be sent
     */
    
    public long calcGross(long qtyNet)
    {
        if (qtyNet <=0)
            return 0;                                    // no point getting past charges if we end up with zero anyway


        long lowerGross = ((qtyNet + getChargeFlat()) * 10000)/
                (10000-chargeBasisPoints);                                      // divides rounding down

        if (calcNet(lowerGross) < qtyNet)
            lowerGross+=1;
        return lowerGross;
    }

    /**
     * Calculates the appropriate asset hash length of genesis so that when encoded as metadata the genesis will
     * fit in metadataMaxLen bytes. For now, set metadataMaxLen to 40 (see Bitcoin's MAX_OP_RETURN_RELAY parameter).
     *
     * @param metadataMaxLen - metadata maximal length
     * @return asset hash length of genesis
     */
    
    public int calcHashLen(int metadataMaxLen)
    {
        int HashLen = metadataMaxLen-COINSPARK_METADATA_IDENTIFIER.length()-1-COINSPARK_GENESIS_QTY_FLAGS_LENGTH;

        if (this.chargeFlatMantissa>0)
            HashLen-=COINSPARK_GENESIS_CHARGE_FLAT_LENGTH;

        if (this.chargeBasisPoints>0)
            HashLen-=COINSPARK_GENESIS_CHARGE_BPS_LENGTH;

        
        CoinSparkAssetWebPage assetWebPage=new CoinSparkAssetWebPage(domainName, pagePath, useHttps, usePrefix);
        HashLen-=assetWebPage.encodedLen();

        if (HashLen > COINSPARK_GENESIS_HASH_MAX_LEN)
            HashLen = COINSPARK_GENESIS_HASH_MAX_LEN;

        return HashLen;
    }
    
    /**
     * Encodes the genesis into metadata (maximal size is CoinSparkBase.OP_RETURN_MAXIMUM_LENGTH);
     * 
     * @return String | null Encoded genesis as hexadecimal, null if we failed.
     */
    /*
    public String encodeToHex()
    {
        return encodeToHex(OP_RETURN_MAXIMUM_LENGTH);
    }
    */
    
    /**
     * Encodes the genesis into metadata (maximal size is metadataMaxLen);
     * 
     * @param metadataMaxLen maximal size of encoded data
     * @return String | null Encoded genesis as hexadecimal, null if we failed.
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
     * Encodes the genesis into metadata (maximal size is CoinSparkBase.OP_RETURN_MAXIMUM_LENGTH);
     * 
     * @return byte [] | null Encoded genesis as raw data, null if we failed.
     */
    /*    
    public byte [] encode()
    {        
        return encode(OP_RETURN_MAXIMUM_LENGTH);
    }
    */
    /**
     * Encodes the genesis into metadata (maximal size is metadataMaxLen);
     * 
     * @param metadataMaxLen maximal size of encoded data
     * @return byte [] | null Encoded genesis as hexadecimal, null if we failed.
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
     * Decodes the genesis in metadata  into paymentRef.
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
     * Decodes the genesis in metadata  into paymentRef.
     * 
     * @param metadata Metadata to decode as raw data
     * @return true on success, false on failure
     */
    
    public boolean decode(byte [] metadata)
    {
        CoinSparkBuffer buffer=new CoinSparkBuffer(metadata);
        return decode(buffer);
    }
    
    
    /**
     * Returns the minimum transaction fee (in bitcoin satoshis) required to make the genesis transaction valid.
     * Pass the number of bitcoin satoshis in each output in outputsSatoshis (array size countOutputs).
     * Use CoinSparkScriptIsRegular() to pass an array of bools in outputsRegular for whether each output script is regular.
     *
     * @param outputsSatoshis array of output values
     * @param outputsRegular pass array of booleans for whether each output script is regular
     * @return minimum transaction fee
     */
    
    public long calcMinFee(long[] outputsSatoshis, boolean[] outputsRegular)
    {
        return getMinFeeBasis(outputsSatoshis, outputsRegular) *
                countNonLastRegularOutputs(outputsRegular);
     }

    /**
     * For the asset specified by genesis, calculate the number of newly created asset units in each
     * output of the genesis transaction into the outputBalances array (size countOutputs).
     * Use CoinSparkScriptIsRegular() to pass an array of bools in outputsRegular for whether each output script is regular.
     * ** This is only relevant if the transaction DOES HAVE a sufficient fee to make the genesis valid **
     * 
     * @param outputsRegular array of bools in outputsRegular for whether each output script is regular
     * @return output balances 
    */
    
    public long [] apply(boolean[] outputsRegular)
    {
        int countOutputs=outputsRegular.length;
        
        long [] outputBalances=new long [countOutputs];
        long qtyPerOutput;

        int lastRegularOutput = getLastRegularOutput(outputsRegular);
        int divideOutputs = countNonLastRegularOutputs(outputsRegular);
        long genesisQty = getQty();

        if (divideOutputs==0)
            qtyPerOutput = 0;
        else
            qtyPerOutput = genesisQty / (divideOutputs); // rounds down

        long extraFirstOutput = genesisQty - (qtyPerOutput * divideOutputs);

        for (int outputIndex=0; outputIndex<countOutputs; outputIndex++)
        {
            outputBalances[outputIndex] = 0;
            if (outputsRegular[outputIndex] && (outputIndex!=lastRegularOutput)) {
                outputBalances[outputIndex] = qtyPerOutput +  extraFirstOutput;
                extraFirstOutput = 0; // so it will only contribute to the first
            } else
                outputBalances[outputIndex] = 0;
        }
        
        return outputBalances;
    }

    /**
     * Calculates the URL for the asset web page of genesis.
     * 
     * @param FirstSpentTxID if path=null or path.length=0 pass the previous txid whose output was spent by the first input of the genesis
     * @param FirstSpentVout if path=null or path.length=0 pass the output index of firstSpentTxID spent by the first input of the genesis
     * @return String | null URL of the Asset web page, null on failure
     */
    
    public String calcAssetURL(String FirstSpentTxID,long FirstSpentVout)
    {
        CoinSparkAssetWebPage assetWebPage=new CoinSparkAssetWebPage(domainName, pagePath, useHttps, usePrefix);
        return assetWebPage.getAssetURL(FirstSpentTxID, FirstSpentVout);
    }    
    
    /**
     * Calculates the URL for the home page, based on domain and useHttps flag
     * 
     * @return String | null URL of the domain home page, null on failure
     */
    
    
    public String getDomainURL()
    {
        CoinSparkAssetWebPage assetWebPage=new CoinSparkAssetWebPage(domainName, pagePath, useHttps, usePrefix);
        return assetWebPage.getDomainURL();
    }    
    
    /**
     * Calculates the assetHash for the key information from a CoinSpark asset web page JSON specification.
     * All char* string parameters except contractContent must be passed using UTF-8 encoding.
     * You may pass NULL (and if appropriate, a length of zero) for any parameter which was not in the JSON.
     * Note that you need to pass in the contract *content* and length, not its URL.
     * To call this you must provide a CoinSparkCalcSHA256Hash() function (prototype below) in your code.
     *
     * @param name asset name
     * @param issuer issuer name
     * @param description asset description
     * @param issueDate issue data
     * @param expiryDate expiry date, if not specified pass null or zero-length String
     * @param interestRate interest rate
     * @param multiple asset multiple
     * @param contractContent contract
     * 
     * @return asset hash or null on failure
    */
    
    public static byte [] calcAssetHash(String name, String issuer, String description, String units, String issueDate, String expiryDate,
                           Double interestRate, Double multiple, byte [] contractContent)
    {
        int bufferSize=1024;
        bufferSize += (name != null) ? name.getBytes().length : 0;
        bufferSize += (issuer != null) ? issuer.getBytes().length : 0;
        bufferSize += (description != null) ? description.getBytes().length : 0;
        bufferSize += (units != null) ? units.getBytes().length : 0;
        bufferSize += (issueDate != null) ? issueDate.getBytes().length : 0;
        bufferSize += (expiryDate != null) ? expiryDate.getBytes().length : 0;
        bufferSize += (contractContent != null) ? contractContent.length : 0;
        
        byte[] buffer = new byte[bufferSize ];
        int offset = 0;
        
        offset+=addToHashBuffer(name, buffer, offset);
        offset+=addToHashBuffer(issuer, buffer, offset);
        offset+=addToHashBuffer(description, buffer, offset);
        offset+=addToHashBuffer(units, buffer, offset);
        offset+=addToHashBuffer(issueDate, buffer, offset);
        offset+=addToHashBuffer(expiryDate, buffer, offset);


        long interestRateToHash = Math.round(Math.floor(((interestRate != null)  ? interestRate : 0)*1000000.0+0.5));
        long multipleToHash = Math.round(Math.floor(((multiple != null) ? multiple : 1)*1000000.0+0.5));
        
        String temp = String.format("%d", interestRateToHash);
        offset+=addToHashBuffer(temp, buffer, offset);
        temp = String.format("%d", multipleToHash);
        offset+=addToHashBuffer(temp, buffer, offset);

        if(contractContent != null)
        {
            System.arraycopy(contractContent, 0, buffer, offset, contractContent.length);
            offset += contractContent.length+1;buffer[offset-1]=0x00;
        }
        else
        {
            offset+=addToHashBuffer(null, buffer, offset);
        }

        return coinSparkCalcSHA256Hash(buffer, offset);
    }
    
    /**
     * Compares given hash with value encoded in genesis.
     * 
     * @param AssetHashToCheck asset hash to validate
     * @return true if hashes match, false otherwise
     */
    
    public boolean validateAssetHash(byte [] AssetHashToCheck)
    {
        if(AssetHashToCheck == null)
        {
            return false;
        }

        if(AssetHashToCheck.length < assetHashLen)
        {
            return false;
        }
        
        byte [] arr1=Arrays.copyOf(assetHash, assetHashLen);
        byte [] arr2=Arrays.copyOf(AssetHashToCheck, assetHashLen);
        
        return Arrays.equals(arr1, arr2);        
    }

    
// Private variables/constants/functions       
        
    
    private static final int      COINSPARK_GENESIS_QTY_FLAGS_LENGTH = 2;
    private static final int      COINSPARK_GENESIS_QTY_MASK = 0x3FFF;
    private static final int      COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE = 1001;
    private static final int      COINSPARK_GENESIS_FLAG_CHARGE_FLAT = 0x4000;
    private static final int      COINSPARK_GENESIS_FLAG_CHARGE_BPS = 0x8000;
    private static final int      COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE = 101;
    private static final int      COINSPARK_GENESIS_CHARGE_FLAT_LENGTH = 1;
    private static final int      COINSPARK_GENESIS_CHARGE_BPS_LENGTH = 1;

    private static final int      COINSPARK_GENESIS_QTY_MANTISSA_MIN                  = 1;
    private static final short    COINSPARK_GENESIS_QTY_MANTISSA_MAX                  = 1000;
    private static final int      COINSPARK_GENESIS_QTY_EXPONENT_MIN                  = 0;
    private static final short    COINSPARK_GENESIS_QTY_EXPONENT_MAX                  = 11;
    private static final int      COINSPARK_GENESIS_CHARGE_FLAT_MAX                   = 5000;
    private static final int      COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN = 0;
    private static final short    COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX = 100;
    private static final int      COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX = 50;
    private static final int      COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN = 0;
    private static final short    COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX = 2;
    private static final int      COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN = 0;
    private static final int      COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX = 250;
    private static final int      COINSPARK_GENESIS_HASH_MIN_LEN = 12;
    private static final int      COINSPARK_GENESIS_HASH_MAX_LEN = 32;


    private short qtyExponent;
    private short qtyMantissa;
    private short chargeFlatMantissa;
    private short chargeFlatExponent;
    private short chargeBasisPoints; // one hundredths of a percent
    private byte[] assetHash = new byte[COINSPARK_GENESIS_HASH_MAX_LEN];
    private int assetHashLen; // number of bytes in assetHash that are valid for comparison
//    private CoinSparkAssetWebPage assetWebPage;
    private String domainName;
    private String pagePath;
    private boolean useHttps;
    private boolean usePrefix;


    
    private boolean encode(CoinSparkBuffer buffer,int metadataMaxLen)
    {
        try
        {
            if (!isValid())
                throw new CoinSparkExceptions.CannotEncode("invalid genesis");

            buffer.writeString(COINSPARK_METADATA_IDENTIFIER);
            buffer.writeByte(COINSPARK_GENESIS_PREFIX);
            
            //  Quantity mantissa and exponent
            
            int quantityEncoded = (this.qtyExponent * COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE + this.qtyMantissa) &
                    COINSPARK_GENESIS_QTY_MASK;
            if (chargeFlatMantissa>0)
                quantityEncoded|=COINSPARK_GENESIS_FLAG_CHARGE_FLAT;
            if (chargeBasisPoints>0)
                quantityEncoded|=COINSPARK_GENESIS_FLAG_CHARGE_BPS;
            
            buffer.writeInt(quantityEncoded, COINSPARK_GENESIS_QTY_FLAGS_LENGTH);

            //  Charges - flat and basis points

            if ((quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_FLAT) != 0)
            {
                int chargeEncoded=this.chargeFlatExponent*COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE+this.chargeFlatMantissa;

                buffer.writeInt(chargeEncoded, COINSPARK_GENESIS_CHARGE_FLAT_LENGTH);
            }

            if ((quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_BPS) != 0)
            {
                buffer.writeInt(chargeBasisPoints, COINSPARK_GENESIS_CHARGE_BPS_LENGTH);
            }

            //  Asset web page
            CoinSparkAssetWebPage assetWebPage=new CoinSparkAssetWebPage(domainName, pagePath, useHttps, usePrefix);
            if (!assetWebPage.encode(buffer))
                throw new CoinSparkExceptions.CannotEncode("cannot write domain name/path");

            //  Asset hash
            
            buffer.writeBytes(assetHash, assetHashLen);
            
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

    
    private boolean decode(CoinSparkBuffer buffer)
    {
        int quantityEncoded, chargeEncoded;
        if(!buffer.locateRange(COINSPARK_GENESIS_PREFIX))
            return false;

        try
        {            
            //  Quantity mantissa and exponent
            
            if(buffer.canRead(COINSPARK_GENESIS_QTY_FLAGS_LENGTH))
            {
                quantityEncoded = buffer.readInt(COINSPARK_GENESIS_QTY_FLAGS_LENGTH);

                qtyMantissa = (short)((quantityEncoded&COINSPARK_GENESIS_QTY_MASK) % COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE);
                qtyExponent = (short)((quantityEncoded&COINSPARK_GENESIS_QTY_MASK) / COINSPARK_GENESIS_QTY_EXPONENT_MULTIPLE);

                if ((qtyMantissa < COINSPARK_GENESIS_QTY_MANTISSA_MIN) ||
                        (qtyMantissa > COINSPARK_GENESIS_QTY_MANTISSA_MAX) )
                    throw new CoinSparkExceptions.CannotDecode("mantissa out of range");

                if ((qtyExponent < COINSPARK_GENESIS_QTY_EXPONENT_MIN) ||
                        (qtyExponent > COINSPARK_GENESIS_QTY_EXPONENT_MAX) )
                    throw new CoinSparkExceptions.CannotDecode("exponent out of range");
            }
            else
                throw new CoinSparkExceptions.CannotDecode("cannot read genesis flags");

            //  Charges - flat and basis points
            
            if ((quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_FLAT) !=0)
            {
                if(buffer.canRead(COINSPARK_GENESIS_CHARGE_FLAT_LENGTH))
                {
                    chargeEncoded = buffer.readInt(COINSPARK_GENESIS_CHARGE_FLAT_LENGTH);

                    this.chargeFlatMantissa = (short)(chargeEncoded % COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE);
                    this.chargeFlatExponent = (short)(chargeEncoded / COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MULTIPLE);

                    if ( (this.chargeFlatExponent<COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MIN) ||
                            (this.chargeFlatExponent>COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) )
                        throw new CoinSparkExceptions.CannotDecode("flat exponent out of range");

                    if (this.chargeFlatMantissa<COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MIN)
                        throw new CoinSparkExceptions.CannotDecode("flat mantissa out of range");

                    if (this.chargeFlatMantissa > (
                            (this.chargeFlatExponent==COINSPARK_GENESIS_CHARGE_FLAT_EXPONENT_MAX) ?
                                    COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX_IF_EXP_MAX : COINSPARK_GENESIS_CHARGE_FLAT_MANTISSA_MAX))
                        throw new CoinSparkExceptions.CannotDecode("flat mantissa out of range");

                }
                else
                    throw new CoinSparkExceptions.CannotDecode("cannot read charge flat");

            }
            else
            {
                this.chargeFlatMantissa=0;
                this.chargeFlatExponent=0;
            }

            if ((quantityEncoded & COINSPARK_GENESIS_FLAG_CHARGE_BPS) != 0)
            {
                if(buffer.canRead(COINSPARK_GENESIS_CHARGE_BPS_LENGTH))
                {
                    this.chargeBasisPoints = buffer.readInt(COINSPARK_GENESIS_CHARGE_BPS_LENGTH).shortValue();

                    if ( (this.chargeBasisPoints<COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MIN) ||
                            (this.chargeBasisPoints>COINSPARK_GENESIS_CHARGE_BASIS_POINTS_MAX) )
                        throw new CoinSparkExceptions.CannotDecode("basic points out of range");

                }
                else
                    throw new CoinSparkExceptions.CannotDecode("cannot read basic points");

            } else
                this.chargeBasisPoints=0;

            //  Domain name
            
            CoinSparkAssetWebPage assetWebPage=new CoinSparkAssetWebPage(domainName, pagePath, useHttps, usePrefix);
            if (!assetWebPage.decode(buffer))
                throw new CoinSparkExceptions.CannotDecode("cannot decode domain name");

            domainName=assetWebPage.domainName;
            pagePath=assetWebPage.path;
            useHttps=assetWebPage.useHttps;
            usePrefix=assetWebPage.usePrefix;
            
            //  Hash of key information
            
            assetHashLen = buffer.availableForRead();//TBD loss
            assetHashLen = Math.min(assetHashLen, COINSPARK_GENESIS_HASH_MAX_LEN); // apply maximum

            if (assetHashLen < COINSPARK_GENESIS_HASH_MIN_LEN)
                // not enough hash data
                throw new CoinSparkExceptions.CannotDecode("has data out of range");

            assetHash=buffer.readBytes(assetHashLen);
        }
        catch (Exception ex)
        {
            System.out.print(ex.getMessage());
            return false;
        }
        
        return isValid();
    }

    private static int countNonLastRegularOutputs(boolean[] outputsRegular)
    {        
        int countRegularOutputs, outputIndex;
        int countOutputs=outputsRegular.length;
        countRegularOutputs=0;

        for (outputIndex=0; outputIndex<countOutputs; outputIndex++) {
            if (outputsRegular[outputIndex])
                countRegularOutputs++;
        }

        return countRegularOutputs > 1 ? countRegularOutputs-1 :  0;
    }
    
    private static String trimForHash(String Source)
    {
        if(Source == null)
        {
            return null;
        }
        
        boolean keepTrimming;
        int from = 0;
        int to = Source.length()-1;
        
        keepTrimming = true;
        while(keepTrimming && (from<Source.length()))
        {
            switch (Source.charAt(from))
            {
                case 0x09: case 0x0A: case 0x0D: case 0x20:
                    from++;
                    break;
                default:
                    keepTrimming = false;
                    break;
            }            
        }
        
        keepTrimming = true;
        while(keepTrimming && (to>=0))
        {
            switch (Source.charAt(to))
            {
                case 0x09: case 0x0A: case 0x0D: case 0x20:
                    to--;
                    break;
                default:
                    keepTrimming = false;
                    break;
            }            
        }

        to++;
        
        if(from>=to)
        {
            return null;
        }
        
        return Source.substring(from,to);
    }
    
    private static int addToHashBuffer(String string, byte[] buffer, int offset)
    {
        String trimmed=trimForHash(string);
        if (trimmed != null && trimmed.length() !=0)
        {
            System.arraycopy(trimmed.getBytes(), 0, buffer, offset, trimmed.length());
            buffer[offset+trimmed.length()] = 0x00;
            return trimmed.length()+1;
        }
        else
        {
            buffer[offset]=0x00;
            return 1;
        }
    }
    
}
