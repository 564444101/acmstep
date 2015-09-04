-- phpMyAdmin SQL Dump
-- version 3.4.10.1
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2015 年 07 月 07 日 04:35
-- 服务器版本: 5.5.20
-- PHP 版本: 5.3.10

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- 数据库: `acmstep`
--

-- --------------------------------------------------------

--
-- 表的结构 `chapter`
--

CREATE TABLE IF NOT EXISTS `chapter` (
  `cid` int(10) unsigned NOT NULL,
  `chapter_name` varchar(50) NOT NULL,
  `topics` varchar(200) NOT NULL,
  PRIMARY KEY (`cid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `chapter`
--

INSERT INTO `chapter` (`cid`, `chapter_name`, `topics`) VALUES
(1, 'å…¥é—¨', '1,2'),
(2, 'æ›´å¤§çš„æŒ‘æˆ˜', '3'),
(3, 'æ›´å¾®å¦™çš„æŠ€æœ¯', ''),
(4, 'é«˜çº§ç®—æ³•ä¸Žå›°éš¾çš„ä¹ é¢˜', '');

-- --------------------------------------------------------

--
-- 表的结构 `code`
--

CREATE TABLE IF NOT EXISTS `code` (
  `code_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) unsigned NOT NULL,
  `author` varchar(20) NOT NULL,
  `problem_id` int(11) unsigned NOT NULL,
  `code` text NOT NULL,
  `judge_status` tinyint(4) NOT NULL,
  `lang` tinyint(4) NOT NULL,
  `run_time` int(11) NOT NULL,
  `run_memory` int(11) NOT NULL,
  `code_length` int(11) NOT NULL,
  `submit_time` datetime NOT NULL,
  `is_inner` tinyint(4) NOT NULL,
  `type` tinyint(4) NOT NULL COMMENT '0代表休闲专题 1代表天梯关卡',
  `type_id` int(11) NOT NULL COMMENT '如果是休闲关卡表示topic_id 如果是天梯表示ladder_id',
  PRIMARY KEY (`code_id`),
  KEY `user_id` (`user_id`),
  KEY `problem_id` (`problem_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=22 ;

--
-- 转存表中的数据 `code`
--

INSERT INTO `code` (`code_id`, `user_id`, `author`, `problem_id`, `code`, `judge_status`, `lang`, `run_time`, `run_memory`, `code_length`, `submit_time`, `is_inner`, `type`, `type_id`) VALUES
(1, 1, 'zz', 1, '/qqqq/qq', 4, 0, 100, 65, 501, '2014-12-17 00:00:00', 0, 0, 0),
(5, 1, 'zz', 1, '/qqqq/qq', 1, 0, 100, 65, 501, '2014-12-17 00:00:00', 0, 0, 0),
(6, 3, 'zz', 1, 'sdfsdf								', 4, 0, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(7, 3, 'zz', 1, '						', 0, 0, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(8, 3, 'zz', 1, 'sdfsadfwqef								', 0, 0, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(9, 3, 'zz', 1, 'yyy					', 0, 0, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(10, 3, 'zz', 1, 'yyy					', 0, 1, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(11, 3, 'zz', 1, 'sdfasdf								', 0, 0, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(12, 3, 'zz', 1, '								jklkjlkj', 0, 0, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(13, 3, 'zz', 1, 'sdfasdfasdf								', 0, 0, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(14, 3, 'zz', 1, '								', 0, 0, 0, 0, 0, '0000-00-00 00:00:00', 0, 0, 0),
(15, 3, 'zz', 1, '								a+b', 0, 0, 0, 0, 0, '2015-05-09 06:18:40', 0, 0, 0),
(16, 3, 'zz', 1, 'ä¸ºèŒƒæ–‡èŠ³								a+b', 0, 0, 0, 0, 0, '2015-05-09 06:19:13', 0, 0, 0),
(17, 3, 'zz', 1, 'è¿æ³•ä¸ºèŒƒæ–‡èŠ³								a+b', 0, 0, 0, 0, 0, '2015-05-09 06:20:03', 0, 0, 0),
(18, 3, 'zz', 1, 'è¿æ³•ä¸ºèŒƒæ–‡èŠ³								a+b', 0, 0, 0, 0, 0, '2015-05-09 02:22:17', 0, 0, 0),
(19, 3, 'zz', 1, 'è¿æ³•ä¸ºèŒƒæ–‡èŠ³								a+b', 0, 0, 0, 0, 0, '2015-05-09 02:22:44', 0, 0, 0),
(20, 3, 'zz', 1, 'è¿æ³•ä¸ºèŒƒæ–‡èŠ³								a+b', 0, 0, 0, 0, 0, '2015-05-09 14:23:11', 0, 0, 0),
(21, 3, 'zz', 1, '								', 0, 0, 0, 0, 0, '2015-05-10 16:30:42', 0, 0, 0);

-- --------------------------------------------------------

--
-- 表的结构 `ladder`
--

CREATE TABLE IF NOT EXISTS `ladder` (
  `ladder_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `problem_id` int(11) unsigned DEFAULT NULL,
  `ladder_name` varchar(200) NOT NULL COMMENT '关卡名称',
  PRIMARY KEY (`ladder_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- 转存表中的数据 `ladder`
--

INSERT INTO `ladder` (`ladder_id`, `problem_id`, `ladder_name`) VALUES
(1, 1, 'a+ b');

-- --------------------------------------------------------

--
-- 表的结构 `medal`
--

CREATE TABLE IF NOT EXISTS `medal` (
  `medal_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `topic_id` int(10) unsigned NOT NULL,
  `medal_name` varchar(30) NOT NULL,
  `medal_path` varchar(30) DEFAULT 'medal_path',
  PRIMARY KEY (`medal_id`),
  UNIQUE KEY `module_id` (`topic_id`,`medal_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- 表的结构 `problem`
--

CREATE TABLE IF NOT EXISTS `problem` (
  `time_limit` int(10) unsigned NOT NULL DEFAULT '1000',
  `memory_limit` int(10) unsigned NOT NULL DEFAULT '65536',
  `problem_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `problem_name` varchar(50) NOT NULL,
  `description` text,
  `input` text,
  `output` text,
  `sample_input` text,
  `sample_output` text,
  `hint` text,
  `solve_times` int(10) unsigned DEFAULT NULL,
  `submit_times` int(10) unsigned DEFAULT NULL,
  `special_judge` tinyint(1) NOT NULL DEFAULT '0',
  `is_inner` tinyint(4) NOT NULL COMMENT '是否为内部题目',
  `tip` text NOT NULL COMMENT '锦囊（题解）',
  `tip_code` text NOT NULL COMMENT '题解标程',
  `oj_id` int(11) NOT NULL DEFAULT '0' COMMENT '表示外部OJ名字的ID',
  `outer_oj_pid` int(11) NOT NULL DEFAULT '1000' COMMENT '外部OJ题号',
  PRIMARY KEY (`problem_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=19 ;

--
-- 转存表中的数据 `problem`
--

INSERT INTO `problem` (`time_limit`, `memory_limit`, `problem_id`, `problem_name`, `description`, `input`, `output`, `sample_input`, `sample_output`, `hint`, `solve_times`, `submit_times`, `special_judge`, `is_inner`, `tip`, `tip_code`, `oj_id`, `outer_oj_pid`) VALUES
(32768, 64, 1, 'A + B Problem', '<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">Calculate&nbsp;</span><i>A + B</i><span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">.</span>', '<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">Each line will contain two integers&nbsp;</span><i>A</i><span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">&nbsp;and&nbsp;</span><i>B</i><span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">. Process to end of file.</span>', '<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">For each case, output&nbsp;</span><i>A + B</i><span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">&nbsp;in one line.</span>', '<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;line-height:normal;">1 1</span>', '<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;line-height:normal;">2</span>', '', 0, 0, 0, 1, '', '', 0, 1000),
(32768, 64, 2, 'Sum Problem', '<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">Hey, welcome to HDOJ(Hangzhou Dianzi University Online Judge).</span><br />\r\n<br />\r\n<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">In this problem, your task is to calculate SUM(n) = 1 + 2 + 3 + ... + n.</span>', '<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">The input will consist of a series of integers n, one integer per line.</span>', '<span style="font-family:''Times New Roman'';font-size:14px;">For each case, output SUM(n) in one line, followed by a blank line. You may assume the result will be in the range of 32-bit signed integer.</span><br />', '<p>\r\n	<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;">1&nbsp;</span>\r\n</p>\r\n<p>\r\n	<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;">100</span>\r\n</p>', '<p>\r\n	<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;line-height:normal;">1&nbsp;</span>\r\n</p>\r\n<p>\r\n	<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;line-height:normal;">5050</span>\r\n</p>', '', 0, 0, 0, 1, '', '', 0, 1000),
(32768, 64, 3, 'A + B Problem II', '<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">I have a very simple problem for you. Given two integers A and B, your job is to calculate the Sum of A + B.</span>', '<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">The first line of the input contains an integer T(1&lt;=T&lt;=20) which means the number of test cases. Then T lines follow, each line consists of two positive integers, A and B. Notice that the integers are very large, that means you should not process them by using 32-bit integer. You may assume the length of each integer will not exceed 1000.</span>', '<span style="font-family:''Times New Roman'';font-size:14px;line-height:normal;">For each test case, you should output two lines. The first line is "Case #:", # means the number of the test case. The second line is the an equation "A + B = Sum", Sum means the result of A + B. Note there are some spaces int the equation. Output a blank line between two test cases.</span>', '<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;line-height:normal;">2\r\n1 2\r\n112233445566778899 998877665544332211</span>', '<p>\r\n	<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;">Case 1:\r\n1 + 2 = 3&nbsp;</span>\r\n</p>\r\n<p>\r\n	<span style="font-family:''Courier New'', Courier, monospace;font-size:14px;">Case 2:\r\n112233445566778899 + 998877665544332211 = 1111111111111111110</span>\r\n</p>', '', 0, 0, 0, 1, '', '', 0, 1000),
(32768, 64, 4, 'Probability One', 'Number guessing is a popular game between elementary-school kids. Teachers encourage pupils to play the game as it enhances their arithmetic skills, logical thinking, and following-up simple procedures. We think that, most probably, you too will master in few minutes. Hereâ€™s one example of how you too can play this game: Ask a friend to think of a number, letâ€™s call it n0.<br />\r\nThen:<br />\r\n1. Ask your friend to compute n1 = 3 * n0 and to tell you if n1 is even or odd.<br />\r\n2. If n1 is even, ask your friend to compute n2 = n1/2. If, otherwise, n1 was odd then let your friend compute n2 = (n1 + 1)/2.<br />\r\n3. Now ask your friend to calculate n3 = 3 * n2.<br />\r\n4. Ask your friend to tell tell you the result of n4 = n3/9. (n4 is the quotient of the division operation. In computer lingo, â€™/â€™ is the integer-division operator.)<br />\r\n5. Now you can simply reveal the original number by calculating n0 = 2 * n4 if n1 was even, or n0 = 2 * n4 + 1 otherwise.<br />\r\nHereâ€™s an example that you can follow: If n0 = 37, then n1 = 111 which is odd. Now we can calculate n2 = 56, n3= 168, and n4 = 18, which is what your friend will tell you. Doing the calculation 2 Ã— n4 + 1 = 37 reveals n0.<br />\r\n<br />', 'ur program will be tested on one or more test cases. Each test case is made of a single <br />\r\npositive number (0 &lt; n0 &lt; 1, 000, 000).<br />\r\nThe last line of the input file has a single zero (which is not part of the test cases.)<br />', 'For each test case, print the following line:<br />\r\nk. B Q<br />\r\nWhere k is the test case number (starting at one,) B is either â€™evenâ€™ or â€™oddâ€™ (without the quotes) depending on your friendâ€™s answer in step 1. Q is your friendâ€™s answer to step 4.<br />\r\nNote: There is a blank space before B.<br />', '37<br />\r\n38<br />\r\n0<br />', '1. odd 18<br />\r\n2. even 19<br />', '', 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 7, '6767', '<span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span><span>sdfasdfasdfqwefqwef</span></span><span></span><span>sdfasdfasdfqwefqwef</span>', '', '', '', '', '', 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 8, '33131', '3', '3', '44444', '3', '112121', '3', 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 9, '31111', '3', '3', '3', '3', '3', '3', 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 10, '44', 'hahahahahh', '							44121 ', '						    44444 ', '							44 ', '							44 ', NULL, 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 12, 'bbz', '', NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 14, '777', '', NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 15, '45545', '', NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 16, 'wef', '', NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 17, 'wefwfwefq', '', NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 1, '', '', 0, 1000),
(1000, 64, 18, 'w3', '', NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 1, '', '', 0, 1000);

-- --------------------------------------------------------

--
-- 表的结构 `tip`
--

CREATE TABLE IF NOT EXISTS `tip` (
  `user_id` int(11) NOT NULL,
  `open_time` datetime NOT NULL,
  `problem_id` int(11) NOT NULL,
  PRIMARY KEY (`user_id`,`problem_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `tip`
--

INSERT INTO `tip` (`user_id`, `open_time`, `problem_id`) VALUES
(1, '0000-00-00 00:00:00', 1),
(1, '0000-00-00 00:00:00', 2);

-- --------------------------------------------------------

--
-- 表的结构 `topic`
--

CREATE TABLE IF NOT EXISTS `topic` (
  `topic_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `cid` int(11) NOT NULL COMMENT '所属章节ID',
  `pre_topic` int(10) unsigned NOT NULL COMMENT '前置关卡',
  `topic_name` varchar(50) NOT NULL,
  `problem_ids` varchar(300) NOT NULL,
  `medal_id` int(10) unsigned NOT NULL,
  `pass_num` tinyint(3) unsigned NOT NULL COMMENT '过关所需题数',
  `description` varchar(200) NOT NULL COMMENT '专题描述',
  UNIQUE KEY `topic_id` (`topic_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=5 ;

--
-- 转存表中的数据 `topic`
--

INSERT INTO `topic` (`topic_id`, `cid`, `pre_topic`, `topic_name`, `problem_ids`, `medal_id`, `pass_num`, `description`) VALUES
(1, 1, 1, 'åŸºæœ¬è¾“å…¥è¾“å‡º', '1001,1002,1,2,3,7,8,9,16,4', 0, 3, 'è¿™æ˜¯ä¸€ä¸ªä¸é”™çš„ä¸“é¢˜ å‘µå‘µ'),
(2, 1, 1, 'a+bè®­ç»ƒ', '', 0, 2, ''),
(3, 2, 2, '3', '1', 0, 1, 'wefwefw'),
(4, 3, 3, 'äºŒåˆ†å’Œè´ªå¿ƒ', '8', 0, 5, 'å¥½ä¸“é¢˜');

-- --------------------------------------------------------

--
-- 表的结构 `user`
--

CREATE TABLE IF NOT EXISTS `user` (
  `user_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `head_photo` varchar(60) DEFAULT NULL,
  `username` varchar(20) NOT NULL,
  `password` varchar(20) NOT NULL,
  `motto` varchar(100) DEFAULT NULL,
  `topic_solved_times` int(11) unsigned NOT NULL DEFAULT '0',
  `ladder_solved_times` int(10) unsigned NOT NULL DEFAULT '0',
  `topic_submited_times` int(11) unsigned NOT NULL DEFAULT '0',
  `ladder_submited_times` int(10) unsigned NOT NULL DEFAULT '0',
  `sex` enum('1','2','3') NOT NULL DEFAULT '3',
  `nickname` varchar(30) DEFAULT NULL,
  `realname` varchar(30) CHARACTER SET utf8 COLLATE utf8_esperanto_ci DEFAULT NULL,
  `age` smallint(6) DEFAULT NULL,
  `school` varchar(30) DEFAULT NULL,
  `class` varchar(30) DEFAULT NULL,
  `email` varchar(60) DEFAULT NULL,
  `qq` varchar(20) DEFAULT NULL,
  `major` varchar(20) DEFAULT NULL,
  `pass_topics` varchar(200) DEFAULT NULL,
  `at_topics` varchar(100) NOT NULL DEFAULT '1' COMMENT '用户所处休闲关卡',
  `at_ladder` int(11) NOT NULL DEFAULT '1' COMMENT '用户所处天梯关卡',
  `medallist` varchar(100) DEFAULT NULL COMMENT '用户徽章列表',
  `user_type` tinyint(4) NOT NULL DEFAULT '0',
  `register_time` datetime NOT NULL COMMENT '注册时间',
  `topic_rank` int(10) unsigned DEFAULT NULL COMMENT '训练场排名',
  `ladder_rank` int(11) DEFAULT NULL COMMENT '天梯排名',
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `uid` (`user_id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=12 ;

--
-- 转存表中的数据 `user`
--

INSERT INTO `user` (`user_id`, `head_photo`, `username`, `password`, `motto`, `topic_solved_times`, `ladder_solved_times`, `topic_submited_times`, `ladder_submited_times`, `sex`, `nickname`, `realname`, `age`, `school`, `class`, `email`, `qq`, `major`, `pass_topics`, `at_topics`, `at_ladder`, `medallist`, `user_type`, `register_time`, `topic_rank`, `ladder_rank`) VALUES
(3, 'public/headphoto/zz1422327135.jpg', 'zz', 'zz', 'zz', 3, 5, 9, 0, '3', 'wind', 'zz', 23, 'hrbust', '1101', '232323@qq.com', '12346789', 'cs', '1,2', '3', 1, '', 1, '0000-00-00 00:00:00', 0, 0),
(7, NULL, 't', 't', NULL, 0, 0, 0, 0, '3', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', 0, '', 0, '0000-00-00 00:00:00', 0, 0),
(8, 'public/headphoto/wind1421998896.jpg', 'wind', 'wind', 'ä»Šå¤©æ‰æ˜¯æœ€å¥½çš„ç¤¼ç‰©(^_^)Y', 0, 0, 0, 0, '3', '', '', 0, '', '', '', '', '', NULL, '', 0, '', 0, '0000-00-00 00:00:00', 0, 0),
(9, NULL, 'zhou', 'zhou', NULL, 0, 0, 0, 0, '3', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', 0, '', 0, '0000-00-00 00:00:00', 0, 0),
(10, NULL, 'Griselda', 'zhouzhou', NULL, 0, 0, 0, 0, '3', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', 0, '', 0, '0000-00-00 00:00:00', 0, 0),
(11, NULL, 'zztmp', 'zhouzhou', NULL, 0, 0, 0, 0, '3', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '1', 1, NULL, 0, '2015-05-21 06:56:19', NULL, NULL);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
