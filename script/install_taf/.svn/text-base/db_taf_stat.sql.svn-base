-- MySQL dump 10.9
--
-- Host: localhost    Database: db_taf_stat
-- ------------------------------------------------------
-- Server version	4.1.15-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `t_master`
--

DROP TABLE IF EXISTS `t_master`;
CREATE TABLE `t_master` (
  `master_name` char(128) NOT NULL default '',
  PRIMARY KEY  (`master_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk ROW_FORMAT=FIXED;

--
-- Table structure for table `t_master_ip`
--

DROP TABLE IF EXISTS `t_master_ip`;
CREATE TABLE `t_master_ip` (
  `master_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(128) NOT NULL default '',
  PRIMARY KEY  (`master_name`,`master_ip`(1))
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_master_property`
--

DROP TABLE IF EXISTS `t_master_property`;
CREATE TABLE `t_master_property` (
  `master_name` varchar(50) NOT NULL default '',
  `property_name` varchar(60) NOT NULL default '',
  `policy` varchar(20) NOT NULL default '',
  PRIMARY KEY  (`master_name`,`property_name`,`policy`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_master_slave`
--

DROP TABLE IF EXISTS `t_master_slave`;
CREATE TABLE `t_master_slave` (
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  PRIMARY KEY  (`master_name`,`slave_name`(1))
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_master_slave_interface`
--

DROP TABLE IF EXISTS `t_master_slave_interface`;
CREATE TABLE `t_master_slave_interface` (
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  PRIMARY KEY  (`master_name`,`slave_name`,`interface_name`(1))
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
--
