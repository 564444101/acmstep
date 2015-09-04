<?php
    class TopicModel extends Model{
        function __construct(){
            parent::__construct();
            $this->_tableName = 'topic';
            $this->_tablePk = 'topic_id';
        }

		function addTopic($row){
			dump($row);
			return $this->add($row);
		}
	
		function delTopic($tid){
			$conditions = array();
			$conditions['topic_id'] = $tid;	
			return $this->del($conditions);
		}

		function getTopicList(){
				
			return $this->findAll();
		}

		function getTopicInfo($tid){
			$field = 'topic_id';
			return $this->findBy($field, $tid);
		}

		function updateTopicField($tid, $field, $val){
			$conditions = array();
			$conditions['topic_id'] = $tid;
			return $this->updateField($conditions, $field, $val);
		}
		
		function updateTopicInfo($tid, $topicInfo){
			$conditions = array();
			$conditions['topic_id'] = $tid;
			return $this->update($conditions, $topicInfo);
		}
    }
?>
