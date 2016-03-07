-- MySQL dump 10.9
--
-- Host: localhost    Database: db_taf
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
-- Table structure for table `t_adapter_conf`
--

DROP TABLE IF EXISTS `t_adapter_conf`;
CREATE TABLE `t_adapter_conf` (
  `id` int(11) NOT NULL auto_increment,
  `application` varchar(128) default '',
  `server_name` varchar(128) default '',
  `node_name` varchar(128) default '',
  `adapter_name` varchar(64) default '',
  `thread_num` int(11) default '1',
  `endpoint` varchar(128) default '',
  `max_connections` int(11) default '1000',
  `allow_ip` varchar(255) NOT NULL default '',
  `servant` varchar(128) default '',
  `queuecap` int(11) default NULL,
  `queuetimeout` int(11) default NULL,
  `posttime` datetime default '0000-00-00 00:00:00',
  `lastuser` varchar(30) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `application` (`application`,`server_name`,`node_name`,`adapter_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_ats_cases`
--

DROP TABLE IF EXISTS `t_ats_cases`;
CREATE TABLE `t_ats_cases` (
  `id` int(11) NOT NULL auto_increment,
  `casename` varchar(20) default NULL,
  `retvalue` text,
  `paramvalue` text,
  `interfaceid` int(11) default NULL,
  `posttime` datetime default NULL,
  `lastuser` varchar(30) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_ats_interfaces`
--

DROP TABLE IF EXISTS `t_ats_interfaces`;
CREATE TABLE `t_ats_interfaces` (
  `id` int(11) NOT NULL auto_increment,
  `objname` varchar(255) default NULL,
  `funcname` varchar(255) default NULL,
  `retype` text,
  `paramtype` text,
  `outparamtype` text,
  `interfaceid` int(11) default NULL,
  `postime` datetime default NULL,
  `lastuser` varchar(30) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_config_files`
--

DROP TABLE IF EXISTS `t_config_files`;
CREATE TABLE `t_config_files` (
  `id` int(11) NOT NULL auto_increment,
  `server_name` varchar(128) default '',
  `host` varchar(20) NOT NULL default '',
  `filename` varchar(128) default NULL,
  `config` longtext,
  `posttime` datetime default NULL,
  `lastuser` varchar(50) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `application` (`server_name`,`filename`,`host`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_config_history_files`
--

DROP TABLE IF EXISTS `t_config_history_files`;
CREATE TABLE `t_config_history_files` (
  `id` int(11) NOT NULL auto_increment,
  `configid` int(11) default NULL,
  `reason` varchar(128) default '',
  `reason_select` varchar(20) NOT NULL default '',
  `content` longtext,
  `posttime` datetime default NULL,
  `lastuser` varchar(50) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_node_info`
--

DROP TABLE IF EXISTS `t_node_info`;
CREATE TABLE `t_node_info` (
  `id` int(11) NOT NULL auto_increment,
  `node_name` varchar(128) default '',
  `node_obj` varchar(128) default '',
  `endpoint_ip` varchar(16) default '',
  `endpoint_port` int(11) default '0',
  `data_dir` varchar(128) default '',
  `load_avg1` float default '0',
  `load_avg5` float default '0',
  `load_avg15` float default '0',
  `last_reg_time` datetime default '1970-01-01 00:08:00',
  `last_heartbeat` datetime default '1970-01-01 00:08:00',
  `setting_state` enum('active','inactive') default 'inactive',
  `present_state` enum('active','inactive') default 'inactive',
  `modify_time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `node_name` (`node_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_profile_template`
--

DROP TABLE IF EXISTS `t_profile_template`;
CREATE TABLE `t_profile_template` (
  `id` int(11) NOT NULL auto_increment,
  `template_name` varchar(128) default '',
  `parents_name` varchar(128) default '',
  `profile` text NOT NULL,
  `posttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastuser` varchar(30) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `template_name` (`template_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_registry_info`
--

DROP TABLE IF EXISTS `t_registry_info`;
CREATE TABLE `t_registry_info` (
  `id` int(11) NOT NULL auto_increment,
  `locator_id` varchar(128) NOT NULL default '',
  `servant` varchar(128) NOT NULL default '',
  `endpoint` varchar(128) NOT NULL default '',
  `last_heartbeat` datetime default '1970-01-01 00:08:00',
  `present_state` enum('active','inactive') default 'inactive',
  `modify_time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `locator_id` (`locator_id`,`servant`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_server_conf`
--

DROP TABLE IF EXISTS `t_server_conf`;
CREATE TABLE `t_server_conf` (
  `id` int(11) NOT NULL auto_increment,
  `application` varchar(128) default '',
  `server_name` varchar(128) default '',
  `node_name` varchar(128) NOT NULL default '',
  `base_path` varchar(128) default '',
  `exe_path` varchar(128) NOT NULL default '',
  `template_name` varchar(128) NOT NULL default '',
  `setting_state` enum('active','inactive') NOT NULL default 'inactive',
  `present_state` enum('active','inactive','activating','deactivating','destroyed') NOT NULL default 'inactive',
  `process_id` int(11) NOT NULL default '0',
  `patch_version` varchar(128) NOT NULL default '',
  `patch_time` datetime NOT NULL default '0000-00-00 00:00:00',
  `patch_user` varchar(128) NOT NULL default '',
  `posttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastuser` varchar(30) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `application` (`application`,`server_name`,`node_name`),
  UNIQUE KEY `application_2` (`application`,`server_name`,`node_name`),
  KEY `node_name` (`node_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

--
-- Table structure for table `t_server_notifys`
--

DROP TABLE IF EXISTS `t_server_notifys`;
CREATE TABLE `t_server_notifys` (
  `id` int(11) NOT NULL auto_increment,
  `server_name` varchar(30) default NULL,
  `server_id` varchar(50) default NULL,
  `thread_id` varchar(20) default NULL,
  `command` varchar(50) default NULL,
  `result` text,
  `notifytime` datetime default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- Table structure for table `t_server_patchs`
--

DROP TABLE IF EXISTS `t_server_patchs`;
CREATE TABLE `t_server_patchs` (
  `id` int(11) NOT NULL auto_increment,
  `server` varchar(50) default NULL,
  `version` varchar(255) default NULL,
  `tgz` text,
  `update_text` varchar(255) default NULL,
  `reason_select` varchar(255) default NULL,
  `document_complate` varchar(30) default NULL,
  `publish` tinyint(3) default NULL,
  `publish_time` datetime default NULL,
  `upload_time` datetime default NULL,
  `upload_user` varchar(30) default NULL,
  `posttime` datetime default NULL,
  `lastuser` varchar(30) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

