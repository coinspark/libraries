CoinSpark Library for Java

Usage
-----
The recommended way to integrate the CoinSpark library into your application is to use the stable packages which are made availble in the Maven repository.

Add the following lines to your project's pom.xml file:

<dependency>
  <groupId>org.coinspark.library</groupId>
  <artifactId>coinspark-library</artifactId>
  <version>1.0.3</version>
</dependency>


Building
--------
If you want to build locally, simply do the following:

mvn clean install

This will build and place the library into your local maven repository, usually located in ~/.m2/


Tips
----
Change the version number from 1.0.3 to something like 1.0.3-HACKING and make sure you update any dependencies.  If you are hcaking on Sparkbit, update the dependency in the pom.xml of SparkBit and SparkBit's version of BitcoinJ.

