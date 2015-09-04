<?php
class TopicController extends Controller{
	function addTopicPage(){
		$this->checkAdmin();
		$this->display('addtopicpage');
	}

	function addTopic(){
		$this->checkAdmin();
		$topicModel = M('Topic');	
		$TopicInfo = $_POST;
		$tid = $topicModel->addTopic($TopicInfo);
		if ( $tid == false ){
			$this->error('add topic failed',U('Topic','showTopicList').'&pid=1');
		}
		else{
			$this->success('add topic success',U('Topic','showTopicList').'&pid=1');
		}
	}

	function delTopic(){
		$this->checkAdmin();
		$topicModel = M('Topic');
		$tid = $_GET['tid'];
		$res = $topicModel->delTopic($tid);
		if ( $res == false ){
			$this->error('del topic failed', U('Topic','showTopicList').'&pid=1');
		}
		else{
			$this->success('del topic success',U('Topic','showTopicList').'&pid=1');
		}
	}

	function showTopicList(){
		$this->checkAdmin();

		$topicModel = M('Topic');
		$chapterModel = M('Chapter');

		$topicRows = $topicModel->getTopicList();
		if ( $topicRows == false ){
			$this->error('get topiclist failed',U('Index','index'));
		}

		$chapterList = $chapterModel->getAllChapter();
		if ( $chapterList == false ){
			$this->error('get Chapterlist failed',U('Index','index'));
		}

		foreach ($chapterList as $chapter){
			$cid = $chapter['cid'];
			$chapterName[$cid] = $chapter['chapter_name'];
		}

		$topicList = array();
		foreach ($topicRows as $topic){
			$cid = $topic['cid'];
			$tid = $topic['topic_id'];
			$topicList[$cid][$tid]['topic_name'] = $topic['topic_name'];
		}
		
		$this->assign('chaptername', $chapterName);
		$this->assign('topiclist', $topicList);
			$this->display('topiclist');
	}

	function editTopicProblem(){
		$this->checkAdmin();
		$tid = $_GET['tid'];			
		$topicModel = M('Topic');
		$topicInfo = $topicModel->getTopicInfo($tid);				
		if (empty($topicInfo)){
			$this->error('get topicinfo failed',U('Topic','showTopicList'));
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
					}
				}
			}
			$this->assign('tid',$tid);
			$this->assign('problemlist',$problemList);
			$this->display('edittopicproblem');
		}	
	}

	function editTopicInfo(){
		$this->checkAdmin();
		$tid = $_GET['tid'];			
		$topicModel = M('Topic');
		$topicInfo = $topicModel->getTopicInfo($tid);				
		if (empty($topicInfo)){
			$this->error('get topicinfo failed',U('Topic','showTopicList'));
		}
		else{
			$this->assign('tid',$tid);
			$this->assign('cid',$topicInfo['cid']);
			$this->assign('pre_topic',$topicInfo['pre_topic']);
			$this->assign('topic_name',$topicInfo['topic_name']);
			$this->assign('pass_num',$topicInfo['pass_num']);
			$this->assign('description',$topicInfo['description']);
			$this->display('edittopicinfo');
		}			
	}

	function updateTopicInfo(){
		$this->checkAdmin();

		$topicModel = M('Topic');
		$topicInfo = $_POST;
		$tid = $_POST['topic_id'];
		if ($topicModel->updateTopicInfo($tid, $topicInfo)){
			$this->success("update topicinfo success");
		}
		else{
			$this->error("update topicinfo failed");
		}
	}

	function addTopicProblem(){
		$this->checkAdmin();
		$tid = $_POST['tid'];
		$pid = $_POST['problem_id'];
		$topicModel = M('Topic');	
		$problemModel = M('Problem');
		$topicInfo = $topicModel->getTopicInfo($tid);
		$problemName = $problemModel->getProblemNameById($pid);
		if (NULL == $problemName){
			$this->error('add Problem failed, problem not existed!'); 
		}
		else{
			$problemIds = $topicInfo['problem_ids'];
			$problemList = explode(',',$topicInfo['problem_ids']);			
			if (in_array($pid, $problemList)){
				$this->error('add Problem failed, problem has existed!');
			}
			if (NULL == $problemIds){
				$problemIds = $pid;		
			}
			else{
				$problemIds = $problemIds.",".$pid;
			}
			if ($topicModel->updateTopicField($tid, 'problem_ids', $problemIds) == true){
				$this->success('Add Problem Success!',U('Topic','editTopicProblem')."&tid=".$tid);
			}
			else{
				$this->error('add Problem failed!'); 
			}
		}
	}

	function delTopicProblem(){
		$this->checkAdmin();
		$tid = $_GET['tid'];
		$pid = $_GET['pid'];		
		$topicModel = M('Topic');	
		$topicInfo = $topicModel->getTopicInfo($tid);
		if (!empty($topicInfo['problem_ids'])){
			$problemList = explode(',',$topicInfo['problem_ids']);			
			$newProblemList = array();
			foreach ($problemList as $id){
				if ( $id != $pid ){
					$newProblemList [] = $id;
				}
			}
			$problemIds = implode(',',$newProblemList);
			if ($topicModel->updateTopicField($tid, 'problem_ids', $problemIds)){
				$this->success('del Problem success!');
			}
			else{
				$this->error('del Problem failed!');
			}
		}
		$this->error('del Problem failed!');
	}

	function getProblemName(){
		$this->checkAdmin();
		$pid = $_GET['pid'];
		$problemModel = M('Problem');
		$pName = $problemModel->getProblemNameById($pid);
		$jsonArr = array('pname'=>$pName);			
		$jsonStr = json_encode($jsonArr);
		echo $jsonStr;
	}
} 
?>
