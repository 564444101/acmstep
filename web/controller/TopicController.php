<?php
    class TopicController extends Controller{

		function showTopicInfo(){
			$this->checkLogin();

			$tid=$_GET['tid'];
			
			$userModel = M('User');
			$uid = $_SESSION['user_id'];
			$userInfo = $userModel->getUserInfo($uid);

			$passTopics = $userInfo['pass_topics'];	
			$atTopics = $userInfo['at_topics'];

			$canOpen = false;
			if (!empty($_SESSION['topiclist'][$tid]) && true == $_SESSION['topiclist'][$tid]){
				$canOpen = true;
			}

			if (false == $canOpen && !empty($passTopics)){
				$passList = explode(',',$passTopics);
				if (in_array($tid, $passList)){
					$canOpen = true;
				}
			}
			if (false == $canOpen && !empty($atTopics)){
				$atList = explode(',',$atTopics);
				if (in_array($tid, $atList)){
					$canOpen = true;
				}
			}
			if (false == $canOpen){
				$this->error('you have no access to this topic ^-^, come on');
			}	
		
			$_SESSION['topiclist'][$tid] = true;

			$topicModel = M('Topic');
			$topicInfo = $topicModel->getTopicInfo($tid);
			if (empty($topicInfo)){
				$this->error('get topicinfo failed ^m^!');
			}
			else{
				$problemList = array();
				if (!empty($topicInfo['problem_ids'])){
					$pList = explode(',',$topicInfo['problem_ids']);    
					$problemModel = M('Problem');
					foreach ($pList as $pid){
						$problemName = $problemModel->getProblemNameById($pid);
						if ($problemName != NULL){
							$problemList[$pid] = $problemName;
							$_SESSION['problemlist'][$pid] = true;
						}   
					}    
				}
			}

			// 获取AC信息，AC数目，提交数目
			$codeModel = M('Code');
			$codeList = $codeModel->getByTypeAndUid(0, $uid);
			
			$acNum = array();
			$subNum = array();
			foreach ($codeList as $code){
				$problemId = $code['problem_id'];
				if (empty($problemList[$problemId]))
					continue;
				if (empty($subNum[$problemId])){
					$subNum[$problemId] = 1;	
					if ($code['judge_status'] == 4){
						$acNum[$problemId] = 1;	
					}
					else{
						$acNum[$problemId] = 0;
					}
				}
				else{
					$subNum[$problemId] += 1;
					if ($code['judge_status'] == 4){
						$acNum[$problemId] += 1;
					}
				}
			}
			
			$this->assign('acnum', $acNum);
			$this->assign('subnum', $subNum);
			$this->assign('userinfo', $userInfo);
			$this->assign('typeid', $tid);
			$this->assign('type', 0);
			$this->assign('problemlist', $problemList);
			$this->display('topicinfo');
		}

		function showTopicList(){
			$this->checkLogin();				
			
			$userModel = M('User');
			$uid = $_SESSION['user_id'];
		    $userInfo = $userModel->getUserInfo($uid);

			$passTopics = $userInfo['pass_topics'];	
			$atTopics = $userInfo['at_topics'];
				
			$topicModel = M('Topic');
			$chapterModel = M('Chapter');

			$chapterName = array();

			$topicRows = $topicModel->getTopicList();

			$chapterList = $chapterModel->getAllChapter();
			if ($chapterList == false){
				$this->error('get chapterlist failed',U('Index','index'));
			}
			
			$chapterType = array();
			foreach ($chapterList as $chapter){
				$cid = $chapter['cid'];
				$chapterName[$cid] = $chapter['chapter_name'];
				$chapterType[$cid] = 3;
			}

			$toipcList = array();
			$topicType = array();

			if (!empty($passTopics)){
				$passList = explode(',',$passTopics);
				foreach ($passList as $tid){
					$topicType[$tid] = 1;
					$_SESSION['topics'][$tid] = true;
				}	
			}

			if (!empty($atTopics)){
				$atList = explode(',',$atTopics);
				foreach ($atList as $tid){
					$topicType[$tid] = 2;
					$_SESSION['topics'][$tid] = true;
				}	
			}

			foreach ($topicRows as $topic){
				$cid = $topic['cid'];
				$tid = $topic['topic_id'];
				$topicName = $topic['topic_name'];
				$topicList[$cid][$tid]['topic_name'] = $topicName;
				if (empty($topicType[$tid])){
					$topicType[$tid] = 3;
					continue;
				}
				if ($topicType[$tid] == 1){
					if ($chapterType[$cid] != 2 ){
						$chapterType[$cid] = 1;
					}	
				} 
				else if ($topicType[$tid] == 2){
					$chapterType[$cid] = 2;
				}
			}		

			$this->assign('userinfo', $userInfo);
			$this->assign('chaptertype', $chapterType);
			$this->assign('topictype', $topicType);
			$this->assign('chaptername', $chapterName);
			$this->assign('topiclist', $topicList);
			$this->display('topic');
		}

    } 
?>
