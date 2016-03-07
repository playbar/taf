-- MySQL dump 10.9
--
-- Host: localhost    Database: db_taf
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
  `grid` int(11) default '0',
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
  `level` int(11) default '2',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `application` (`server_name`,`filename`,`host`,`level`)
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
-- Table structure for table `t_config_references`
--

DROP TABLE IF EXISTS `t_config_references`;
CREATE TABLE `t_config_references` (
  `id` int(11) NOT NULL auto_increment,
  `config_id` int(11) default NULL,
  `reference_id` int(11) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `config_id` (`config_id`,`reference_id`)
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
  `taf_version` varchar(128) NOT NULL default '',
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
  `taf_version` varchar(128) NOT NULL default '',
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
  `taf_version` varchar(128) NOT NULL default '',
  `posttime` datetime NOT NULL default '0000-00-00 00:00:00',
  `lastuser` varchar(30) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `application` (`application`,`server_name`,`node_name`),
  UNIQUE KEY `application_2` (`application`,`server_name`,`node_name`),
  KEY `node_name` (`node_name`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;
alter table `t_server_conf` add monitor_script_path varchar(128) NOT NULL default '' after `node_name`;
alter table `t_server_conf` add stop_script_path varchar(128) NOT NULL default '' after `node_name`;
alter table `t_server_conf` add start_script_path varchar(128) NOT NULL default '' after `node_name`;
alter table `t_server_conf` add server_type enum('taf_cpp','taf_java','not_taf') NOT NULL default 'taf' after `node_name`;
alter table `t_server_conf` add node_group varchar(50) NOT NULL default '' after `server_name`;
alter table `t_server_conf` add bak_flag int(11) NOT NULL default 0 after `template_name`;

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
alter table t_server_patchs add svn_version varchar(255) default NULL after document_complate;

DROP TABLE IF EXISTS `t_web_release_conf`;
CREATE TABLE `t_web_release_conf` (
  `id` int(11) NOT NULL auto_increment,
  `server` varchar(100) NOT NULL default '',
  `path` varchar(200) NOT NULL default '',
  `user` varchar(200) NOT NULL default '*',
  `posttime` datetime default NULL,
  `lastuser` varchar(60) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `IDX_SERVER` (`server`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;
alter table t_web_release_conf add server_dir varchar(200) NOT NULL default '' after path;

DROP TABLE IF EXISTS `t_server_patchs_log`;
CREATE TABLE `t_server_patchs_log` (
  `id` int(11) NOT NULL auto_increment,
  `server` varchar(50) default '',
  `node_ip` varchar(255) default '',
  `update_text` varchar(255) default '',
  `svn_version` varchar(255) default '',
  `result_flag` int(11) default 1, 
  `fail_result` text, 
  `publish_type` enum('rollback','new_version') default 'new_version', 
  `publish_time` datetime default NULL,
  `publish_user` varchar(30) default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;
alter table t_server_patchs_log add patch_group_id int(11) NOT NULL default 0 after id;
alter table t_server_patchs_log add remark varchar(255) NOT NULL default '' after fail_result;

DROP TABLE IF EXISTS `t_server_remark`;
CREATE TABLE `t_server_remark` (
  `id` int(11) NOT NULL auto_increment,
  `application` varchar(128) default '',
  `server_name` varchar(128) default '',
  `file_name` varchar(128) default NULL,
  `remark_content` longtext,
  `posttime` datetime default NULL,
  `lastuser` varchar(60) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `UNIQUE_IDX` (application, server_name, file_name)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_server_remark_history`;
CREATE TABLE `t_server_remark_history` (
  `id` int(11) NOT NULL auto_increment,
  `remark_id` int(11) NOT NULL,
  `application` varchar(128) default '',
  `server_name` varchar(128) default '',
  `file_name` varchar(128) default NULL,
  `remark_content` longtext,
  `posttime` datetime default NULL,
  `lastuser` varchar(60) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_compile_config`;
CREATE TABLE `t_compile_config` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(50) default '',
  `script_path` varchar(128) default '',
  `remark` varchar(128) default '',
  `posttime` datetime default NULL,
  `lastuser` varchar(60) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_server_ping_fails`;
CREATE TABLE `t_server_ping_fails` (
  `id` int(11) NOT NULL auto_increment,
  `server_name` varchar(50) default '',
  `node_name` varchar(255) default '',
  `obj_name` varchar(255) default '',
  `info` varchar(255) default '',
  `fail_time` datetime default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_operation_week_report`;
CREATE TABLE `t_operation_week_report` (
  `period` varchar(20) NOT NULL,
  `events` text not null,
  `server_capability` text not null,
  `recent_works` text not null,
  `info` varchar(255) default '',
  `create_time` datetime default NULL,
  PRIMARY KEY  (`period`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_module_tree_taf_info`;
create table t_module_tree_taf_info (
	`id` int(11) NOT NULL auto_increment,
	`app_group` varchar(50) not null default '',
	`application` varchar(90) not null,
  `pms_application` varchar(90) not null,
  `server_name` varchar(90) not null,
	primary key(server_name,appliction),
        UNIQUE KEY `id` (`id`)
)ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_server_group_relation`;
CREATE TABLE `t_server_group_relation` (
  `id` int(11) NOT NULL auto_increment,
	`application` varchar(90) not null,	
  `server_group` varchar(50) default '',
  `server_name` varchar(50) default '',   
  `create_time` datetime default NULL,
  `creator` varchar(30) default '',
  PRIMARY KEY  (`id`),
  unique key (application,server_name)
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_taf_deploy`;
create table t_taf_deploy (
	`period` varchar(20) not null,
	`app_group` varchar(50) not null default '',
	`application` varchar(90) not null,
	`server_sum` int(11) default 0,
	`storage_sum` int(11) default 0,
	`device_sum` int(11) default 0,
	`single_storage_server_sum` int(11) default 0,
	`single_point_server_sum` int(11) default 0,
	`deploy_plan` varchar(256) not null,
	`principal` varchar(256) not null,
	primary key(period, app_group,appliction)	
 ENGINE=MyISAM DEFAULT CHARSET=gbk;

DROP TABLE IF EXISTS `t_taf_showrun`;
create table t_taf_showrun (
	`period` varchar(20) not null,
	`app_group` varchar(50) not null default '',
	`application` varchar(90) not null,
	`publish_sum` int(11) default 0,
	`exception_restart_sum` int(11) default 0,
	`ping_fail_sum` int(11) default 0,
	`fluctuant_alarm_sum` int(11) default 0,
	`valve_alarm_sum` int(11) default 0,
	`runing_state` varchar(256) not null,
	`principal` varchar(256) not null,
	primary key(period, app_group,appliction)	
 ENGINE=MyISAM DEFAULT CHARSET=gbk;
 
DROP TABLE IF EXISTS `t_svn_list`;
CREATE TABLE `t_svn_list` (
  `id` int(10) NOT NULL default '0',
  `server_name` varchar(200) default NULL,
  `path_name` text,
  `up_date` varchar(30) default NULL,
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk; 

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

