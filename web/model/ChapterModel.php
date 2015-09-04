<?php
    class ChapterModel extends Model{
        function __construct(){
            parent::__construct();
            $this->_tableName = 'chapter';
            $this->_tablePk = 'cid';
        }

		function addChapter($row){
			return $this->add($row);
		}
	
		function delChapter($cid){
			$conditions = array();
			$conditions['cid'] = $cid;
			return $this->del($conditions);
		}

		function getAllChapter(){
			return $this->findAll();		
		}

		function getChapterInfo($cid){
			$field = 'cid';
			return $this->findBy($field, $cid);
		}

		function updateChapterField($cid, $field, $val){
			$conditions = array();
			$conditions['cid'] = $cid;
			return $this->updateField($conditions, $field, $val);
		}
    }
?>
