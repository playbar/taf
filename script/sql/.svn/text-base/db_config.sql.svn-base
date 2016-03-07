-- MySQL dump 10.9
--
-- Host: localhost    Database: db_config
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
-- Table structure for table `t_config_dbconnects`
--

DROP TABLE IF EXISTS `t_config_dbconnects`;
CREATE TABLE `t_config_dbconnects` (
  `ID` smallint(6) unsigned NOT NULL auto_increment,
  `NAME` char(50) default NULL,
  `HOST` char(50) NOT NULL default '',
  `USER` char(50) NOT NULL default '',
  `PASSWORD` char(50) NOT NULL default '',
  `CHARSET` char(30) default NULL,
  `DBNAME` char(50) NOT NULL default '',
  `ABOUT` char(200) default NULL,
  `POSTTIME` datetime default NULL,
  `LASTUSER` char(50) default NULL,
  PRIMARY KEY  (`ID`),
  UNIQUE KEY `IDX_NAME` (`NAME`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk ROW_FORMAT=FIXED;
alter table `t_config_dbconnects` add `PORT` char(10) NOT NULL default '' after `HOST`;   

--
-- Table structure for table `t_config_fields`
--

DROP TABLE IF EXISTS `t_config_fields`;
CREATE TABLE `t_config_fields` (
  `ID` smallint(6) unsigned NOT NULL auto_increment,
  `QUERYID` int(11) NOT NULL default '0',
  `NAME` varchar(50) NOT NULL default '',
  `SEARCHNAME` varchar(50) NOT NULL default '',
  `ABOUT` varchar(100) NOT NULL default '',
  `ISMODIFY` char(1) NOT NULL default '1',
  `ISSEARCH` char(1) default NULL,
  `ISVISIBLE` char(1) default NULL,
  `ISEQUAL` char(1) NOT NULL default '0',
  `ISNULL` char(1) default NULL,
  `ORDERBY` smallint(1) default '0',
  `EDITWIDTH` int(11) default NULL,
  `EDITHEIGHT` int(11) default NULL,
  `SEARCHSIZE` int(11) default NULL,
  `DATATYPE` int(11) default NULL,
  `RAWVALUE` varchar(200) default NULL,
  `OPTIONS` text,
  `POSTTIME` datetime default NULL,
  `LASTUSER` varchar(50) default NULL,
  PRIMARY KEY  (`ID`),
  UNIQUE KEY `IDX_QUERYID_NAME` (`QUERYID`,`NAME`),
  KEY `IDX_QUERYID` (`QUERYID`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_config_menus`
--

DROP TABLE IF EXISTS `t_config_menus`;
CREATE TABLE `t_config_menus` (
  `ID` smallint(6) unsigned NOT NULL auto_increment,
  `PID` smallint(5) unsigned default NULL,
  `NAME` varchar(50) NOT NULL default '',
  `HREF` varchar(255) NOT NULL default '',
  `TYPE` tinyint(3) NOT NULL default '1',
  `ORDERBY` smallint(6) unsigned NOT NULL default '1',
  `POSTTIME` datetime default NULL,
  `LASTUSER` varchar(120) default NULL,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_config_querys`
--

DROP TABLE IF EXISTS `t_config_querys`;
CREATE TABLE `t_config_querys` (
  `ID` smallint(6) unsigned NOT NULL auto_increment,
  `OPNAME` varchar(200) default NULL,
  `QUERYKEY` varchar(20) default NULL,
  `QUERYPOSTTIME` varchar(50) default NULL,
  `QUERYLASTUSER` varchar(50) default NULL,
  `SERVERNAME` varchar(50) NOT NULL default '',
  `DBCONNECTID` smallint(6) NOT NULL default '0',
  `PAGESIZE` tinyint(3) unsigned NOT NULL default '0',
  `SQL` text NOT NULL,
  `ORDERSQL` text NOT NULL,
  `TABLENAME` varchar(50) default NULL,
  `OTHERCALL` text,
  `POSTTIME` datetime default NULL,
  `LASTUSER` varchar(50) default NULL,
  PRIMARY KEY  (`ID`),
  UNIQUE KEY `IDX_OPNAME` (`OPNAME`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_dcache_router_db`;
CREATE TABLE `t_dcache_router_db` (
  `id` int(11) NOT NULL auto_increment,
  `db_connect_id` int(11) NOT NULL,  										# t_config_dbconnectsµÄID
  `app_name` varchar(80) default NULL,
  `memo` text,
  `post_time` datetime default NULL,
  `last_user` varchar(30) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `app_name` (`app_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;


/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

