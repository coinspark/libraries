CoinSpark Library for Java

Usage
-----
The recommended way to integrate the CoinSpark library into your application is to use the stable packages which are made availble in the Maven central repository.

Add the following lines to your project's pom.xml file:

<dependency>
  <groupId>org.coinspark.library</groupId>
  <artifactId>coinspark-library</artifactId>
  <version>1.0.3</version>
</dependency>


Verifying Signatures
--------------------
Packages are signed and you can verify using public key 95B6A985 : http://pgp.mit.edu:11371/pks/lookup?op=get&search=0x1B26FA6F95B6A985

For example,

gpg --verify coinspark-library-1.0.3.jar.asc

Should return:

gpg: Signature made Thu 13 Nov 2014 09:59:39 PM PST using RSA key ID 95B6A985
gpg: Good signature from "Simon Liu (CoinSpark) <simon@coinsciences.com>"


Building
--------
If you want to build locally, simply do the following:

mvn clean install

This will build and place the library into your local maven repository, usually located in ~/.m2/


Tips
----
Change the version number from 1.0.3 to something like 1.0.3-HACKING and make sure you update any dependencies.  If you are hacking on Sparkbit, update the dependency in the pom.xml of SparkBit and SparkBit's version of BitcoinJ.

