-- MySQL dump 10.9
--
-- Host: localhost    Database: db_system
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


DROP TABLE IF EXISTS `t_app_group`;
CREATE TABLE `t_app_group` (
  `group_id` int(11) NOT NULL auto_increment,
  `app_group_name` varchar(50) not null default '',
  `memo` varchar(255) default '',
  `create_time` datetime default NULL,
  `createtor` varchar(30) default NULL,
  PRIMARY KEY  (`group_id`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_app_group_relation`;
CREATE TABLE `t_app_group_relation` (
  `id` int(11) NOT NULL auto_increment,
  `group_id` int(11) NOT NULL,
  `app_id` int(11) NOT NULL,
  `create_time` datetime default NULL,
  `creator` varchar(30) default NULL,
  PRIMARY KEY  (`id`),
  unique key (group_id, app_id)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;


--
-- Table structure for table `t_app`
--

DROP TABLE IF EXISTS `t_app`;
CREATE TABLE `t_app` (
  `id` int(11) NOT NULL auto_increment,
  `application` varchar(90) default NULL,
  `flag` int(11) default NULL,
  `posttime` datetime default NULL,
  `lastuser` varchar(30) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `application` (`application`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_app_users`
--

DROP TABLE IF EXISTS `t_app_users`;
CREATE TABLE `t_app_users` (
  `id` int(11) NOT NULL auto_increment,
  `app` varchar(20) default '',
  `userid` varchar(50) default NULL,
  `posttime` datetime default NULL,
  `lastuser` varchar(50) default NULL,
  PRIMARY KEY  (`id`),
  KEY `IDX_APP` (`app`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_loginers`
--

DROP TABLE IF EXISTS `t_loginers`;
CREATE TABLE `t_loginers` (
  `id` int(11) NOT NULL auto_increment,
  `userid` char(30) NOT NULL default '',
  `nickname` char(50) default NULL,
  `depid` char(5) default NULL,
  `depname` char(80) default NULL,
  `groups` char(255) default NULL,
  `regtime` datetime default NULL,
  `login` int(11) default NULL,
  `lastlogin` datetime default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `IDX_USERID` (`userid`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk ROW_FORMAT=FIXED;

--
-- Table structure for table `t_user_logs`
--

DROP TABLE IF EXISTS `t_user_logs`;
CREATE TABLE `t_user_logs` (
  `id` int(11) NOT NULL auto_increment,
  `userid` varchar(50) default '',
  `app` varchar(50) default NULL,
  `about` varchar(255) default NULL,
  `posttime` datetime default NULL,
  `lastuser` varchar(50) default NULL,
  PRIMARY KEY  (`id`),
  KEY `IDX_USERID` (`userid`),
  KEY `IDX_APP` (`app`),
  KEY `IDX_POSTTIME` (`posttime`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

