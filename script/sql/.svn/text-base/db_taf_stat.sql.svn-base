-- MySQL dump 10.9
--
-- Host: localhost    Database: db_taf_stat
-- ------------------------------------------------------
-- Server version	4.1.15-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES gbk */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `t_ecstatus`
--

DROP TABLE IF EXISTS `t_ecstatus`;
CREATE TABLE `t_ecstatus` (
  `id` int(11) NOT NULL auto_increment,
  `appname` varchar(64) NOT NULL default '',
  `action` tinyint(4) NOT NULL default '0',
  `checkint` smallint(6) NOT NULL default '10',
  `lasttime` varchar(16) NOT NULL default '',
  PRIMARY KEY  (`appname`,`action`),
  UNIQUE KEY `id` (`id`)
) ENGINE=HEAP DEFAULT CHARSET=gbk;

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
-- Table structure for table `t_slave`
--

DROP TABLE IF EXISTS `t_slave`;
CREATE TABLE `t_slave` (
  `slave_name` char(128) NOT NULL default '',
  PRIMARY KEY  (`slave_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk ROW_FORMAT=FIXED;

--
-- Table structure for table `t_slave_ip`
--

DROP TABLE IF EXISTS `t_slave_ip`;
CREATE TABLE `t_slave_ip` (
  `slave_name` varchar(128) NOT NULL default '',
  `slave_ip` varchar(128) NOT NULL default '',
  PRIMARY KEY  (`slave_name`,`slave_ip`(1))
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_slave_master`
--

DROP TABLE IF EXISTS `t_slave_master`;
CREATE TABLE `t_slave_master` (
  `slave_name` varchar(128) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  PRIMARY KEY  (`slave_name`(1),`master_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090313`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090313`;
CREATE TABLE `t_stat_realtime_20090313` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090316`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090316`;
CREATE TABLE `t_stat_realtime_20090316` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090317`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090317`;
CREATE TABLE `t_stat_realtime_20090317` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090318`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090318`;
CREATE TABLE `t_stat_realtime_20090318` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090319`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090319`;
CREATE TABLE `t_stat_realtime_20090319` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090320`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090320`;
CREATE TABLE `t_stat_realtime_20090320` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090321`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090321`;
CREATE TABLE `t_stat_realtime_20090321` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090322`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090322`;
CREATE TABLE `t_stat_realtime_20090322` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090323`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090323`;
CREATE TABLE `t_stat_realtime_20090323` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090324`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090324`;
CREATE TABLE `t_stat_realtime_20090324` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090325`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090325`;
CREATE TABLE `t_stat_realtime_20090325` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090326`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090326`;
CREATE TABLE `t_stat_realtime_20090326` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090327`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090327`;
CREATE TABLE `t_stat_realtime_20090327` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090328`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090328`;
CREATE TABLE `t_stat_realtime_20090328` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090329`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090329`;
CREATE TABLE `t_stat_realtime_20090329` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090330`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090330`;
CREATE TABLE `t_stat_realtime_20090330` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090331`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090331`;
CREATE TABLE `t_stat_realtime_20090331` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_stat_realtime_20090401`
--

DROP TABLE IF EXISTS `t_stat_realtime_20090401`;
CREATE TABLE `t_stat_realtime_20090401` (
  `stattime` timestamp NOT NULL default CURRENT_TIMESTAMP,
  `f_date` date NOT NULL default '1970-01-01',
  `f_tflag` varchar(8) NOT NULL default '',
  `master_name` varchar(128) NOT NULL default '',
  `slave_name` varchar(128) NOT NULL default '',
  `interface_name` varchar(128) NOT NULL default '',
  `master_ip` varchar(15) NOT NULL default '',
  `slave_ip` varchar(15) NOT NULL default '',
  `slave_port` int(10) NOT NULL default '0',
  `return_value` int(11) NOT NULL default '0',
  `succ_count` int(10) unsigned NOT NULL default '0',
  `timeout_count` int(10) unsigned NOT NULL default '0',
  `exce_count` int(10) unsigned NOT NULL default '0',
  `interv_count` varchar(128) default NULL,
  `total_time` int(10) unsigned default NULL,
  `ave_time` int(10) unsigned default NULL,
  `maxrsp_time` int(10) unsigned default NULL,
  `minrsp_time` int(10) unsigned default NULL,
  PRIMARY KEY  (`f_date`,`f_tflag`,`master_name`,`slave_name`,`interface_name`,`master_ip`,`slave_ip`,`slave_port`,`return_value`,`succ_count`,`timeout_count`,`exce_count`),
  KEY `IDX_TIME` (`stattime`),
  KEY `IDC_MASTER` (`master_name`),
  KEY `IDX_INTERFACENAME` (`interface_name`),
  KEY `IDX_SLAVEIP` (`slave_ip`),
  KEY `IDX_SLAVE` (`slave_name`),
  KEY `IDX_RETVALUE` (`return_value`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

