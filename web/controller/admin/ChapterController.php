<?php
class ChapterController extends Controller{
	function addChapterPage(){
		$this->checkAdmin();
		$this->display('addchapterpage');
	}

	function addChapter(){
		$this->checkAdmin();
		$chapterModel = M('Chapter');
		$chapterInfo = $_POST;
		$cid = $_POST['cid'];
		$chapter = $chapterModel->getChapterInfo($cid);
		if (!empty($chapter)){
			$this->error('add chapter failed, the chapter id has existed', U('Topic','showTopicList'));
		}	

		$cid = $chapterModel->addChapter($chapterInfo);
		if ( $cid == false ){
			$this->error('add chapter failed', U('Topic','showTopicList'));
		}
		else{
			$this->success('add chapter success',U('Topic','showTopicList'));
		}
	}

	function editChapter(){
		$this->checkAdmin();
		$cid = $_GET['cid'];
		$chapterModel = M('Chapter');
		$chapterInfo = $chapterModel->getChapterInfo($cid);
		if (empty($chapterInfo)){
			$this->error('get chapterinfo failed, the chapter is not existed',U('Topic','showTopicList'));
		}
		else{
			$this->assign('chapterinfo',$chapterInfo);
			$this->display('editchapter');
		}
	}

	function updateChapterName(){
		$this->checkAdmin();
		$chapterInfo = $_POST;	
		$cid = $_POST['cid'];
		$field = 'chapter_name';
		$val = $_POST['chapter_name'];

		$chapterModel = M('Chapter');			
		if ($chapterModel->updateChapterField($cid, $field, $val)){
			$this->success('update chapter success',U('Topic','showTopicList'));
		}
		else{
			$this->error('update chapter failed',U('Topic','showTopicList'));
		}
	}
} 
?>
