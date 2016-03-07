-- MySQL dump 10.9
--
-- Host: localhost    Database: db_ice
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
-- Table structure for table `t_app_servers`
--

DROP TABLE IF EXISTS `t_app_servers`;
CREATE TABLE `t_app_servers` (
  `ID` int(11) NOT NULL auto_increment,
  `APP` varchar(20) default '',
  `SERVERNAME` varchar(50) default NULL,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_auto_test_cases`
--

DROP TABLE IF EXISTS `t_auto_test_cases`;
CREATE TABLE `t_auto_test_cases` (
  `ID` int(11) NOT NULL auto_increment,
  `CASENAME` varchar(200) default NULL,
  `INTERFACEID` int(11) default NULL,
  `REVALUE` text,
  `PARAMVALUE` text,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_auto_test_interfaces`
--

DROP TABLE IF EXISTS `t_auto_test_interfaces`;
CREATE TABLE `t_auto_test_interfaces` (
  `ID` int(11) unsigned NOT NULL auto_increment,
  `OBJNAME` varchar(100) default NULL,
  `FUNCNAME` varchar(50) default NULL,
  `RETYPE` varchar(50) default NULL,
  `REMODIFY` tinyint(3) default NULL,
  `PARAMTYPE` text,
  `OUTPARAMTYPE` text,
  `POSTTIME` datetime default NULL,
  `LASTUSER` varchar(50) default NULL,
  PRIMARY KEY  (`ID`),
  UNIQUE KEY `IDX_OBJ_FUNC` (`OBJNAME`,`FUNCNAME`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_server_config_historys`
--

DROP TABLE IF EXISTS `t_server_config_historys`;
CREATE TABLE `t_server_config_historys` (
  `ID` int(11) NOT NULL auto_increment,
  `CONFIGID` int(11) NOT NULL default '0',
  `REASON` varchar(255) default NULL,
  `content` longtext,
  `POSTTIME` datetime default NULL,
  `LASTUSER` varchar(50) default NULL,
  `REASON_SELECT` varchar(32) default NULL,
  PRIMARY KEY  (`ID`),
  KEY `IDX_CONFIGID` (`CONFIGID`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--


--
-- Table structure for table `t_server_configs`
--

DROP TABLE IF EXISTS `t_server_configs`;
CREATE TABLE `t_server_configs` (
  `ID` int(11) NOT NULL auto_increment,
  `SERVER` varchar(50) default NULL,
  `FILENAME` varchar(100) default NULL,
  `CONFIG` longtext,
  `POSTTIME` datetime default NULL,
  `LASTUSER` varchar(50) default NULL,
  PRIMARY KEY  (`ID`),
  UNIQUE KEY `IDX_APP_SERVER_FILENAME` (`SERVER`,`FILENAME`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;



--
-- Table structure for table `t_server_notifys`
--

DROP TABLE IF EXISTS `t_server_notifys`;
CREATE TABLE `t_server_notifys` (
  `ID` int(11) NOT NULL auto_increment,
  `TOPIC` varchar(50) default NULL,
  `SERVERID` varchar(200) default NULL,
  `OBJECTID` varchar(80) default NULL,
  `THREADID` int(11) unsigned default NULL,
  `COMMAND` varchar(120) default NULL,
  `RESULT` text,
  `POSTTIME` datetime default NULL,
  PRIMARY KEY  (`ID`),
  KEY `IDX_TOPIC` (`TOPIC`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;



--
-- Table structure for table `t_server_patchs`
--

DROP TABLE IF EXISTS `t_server_patchs`;
CREATE TABLE `t_server_patchs` (
  `ID` int(11) NOT NULL auto_increment,
  `SERVER` varchar(50) default NULL,
  `TGZ` varchar(255) default NULL,
  `UPDATETEXT` text,
  `PUBLISH` char(1) default NULL,
  `PUBLISHTIME` datetime default NULL,
  `UPLOADTIME` datetime default NULL,
  `POSTTIME` datetime default NULL,
  `LASTUSER` varchar(50) default NULL,
  `REASON_SELECT` varchar(32) default NULL,
  `DOCUMENT_COMPLETE` varchar(3) default NULL,
  PRIMARY KEY  (`ID`),
  KEY `IDX_APP` (`SERVER`),
  KEY `IDX_APP_SERVERNAME` (`SERVER`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;



/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

