<?php
namespace lmac\cache;

/**
 * The objects of this class represents cached data units.
 *
 * @author	QuantumG
 */
class CacheItem {
	public $subId;
	public $expire;
	public $itemData; 
	
	/**
	 * Creates a new DataItem object.
	 *
	 * @param $subId
	 *   The id of the item.
	 * @param $expire
	 *   The timestamp of expiration for the item.
	 * @param $data
	 *   The data of the item.
	 */
	public static function create($subId, $expire, $data) {
		$item = new CacheItem();
		$item->subId = $subId;
		$item->expire= $expire;
		$item->itemData = $data;
		
		return $item;
	}
}

class CannotLoadCacheFileException extends \Exception { }
class CannotCreateCacheFiledException extends \Exception { }

/**
 * The objects of this class are used to handle cache pools, in which every sub entry has an own id and can be changed at any time whithout flushing and re-building the entire cache before. 
 * It uses the disc to store cached * data. Each cache bin corresponds to a file by the same name.
 *
 * @author	QuantumG
 */
class CacheController {
	
	CONST EXPIRE_NEVER = -1;
	
	private $pool = [];
	private $cid;
	private $commitRequired = FALSE;
	private $poolFilePath;
	private $cacheFileName;
	
	/**
	 * Constructs a cache pool object.
	 *
	 * @param $cid
	 *   The id of the pool.
	 */
	public function __construct($cid) {

		$poolFilePath = drupal_realpath('private://') . DIRECTORY_SEPARATOR . 'LMACCache';			
		
		$this->cacheFileName = $poolFilePath . DIRECTORY_SEPARATOR . $cid . '.lch';
		$this->cid = $this::sanitizeId($cid);
		$this->_loadPoolFile();
	}
	
	/**
	 * Retrieves all items from the pool.
	 *
	 * @param $defaultItems
	 *   The default items, for the case no items exists in the pool. Either a DataItem when $unpack is FALSE, or the item data when $unpack is TRUE. Default is NULL.
	 * @param $unpack
	 *   When this is TRUE, the item data will be retrieved. Otherwise a DataItem object will be retrieved. Default is FALSE.
	 */
	public function getItems($defaultItems = array(), $unpack = FALSE) {
	
		if (empty($this->pool)) {
			return $defaultItems;
		}
		
		if ($unpack) {
			$unpacked = [];
			foreach($this->pool as $subId=>$cacheItem) {
				$unpacked[$subId] = $cacheItem->itemData;
			}			
			return $unpacked;
		}
	
		return $this->pool;
	}
	
	/**
	 * Retrieves a specific item.
	 *
	 * @param $subId
	 *   The id of the item.
	 * @param $defaultItem
	 *   The default item, for the case the item doesn't exist in the pool. Either a DataItem when $unpack is FALSE, or the item data when $unpack is TRUE. Default is NULL.
	 * @param $unpack
	 *   When this is TRUE, the item data will be retrieved. Otherwise a DataItem object will be retrieved. Default is FALSE.
	 */
	public function getItemBySubId($subId, $defaultItem = NULL, $unpack = FALSE){
		
		$subId = $this::sanitizeId($subId);
 
		if (!array_key_exists($subId, $this->pool)) {
			return $defaultItem;
		}
		
		if ($unpack) {
			return $this->pool[$subId]->itemData;
		}
		
		return $this->pool[$subId];
	}
	
	/**
	 * Updates a specific item.
	 *
	 * @param $subId
	 *   The id of the item.
	 * @param $item
	 *   The data of the item.
	 * @param $expire
	 *   The expiration time of the item. Default is EXPIRE_NEVER, which expires only when the pool is being flushed.
	 */
	public function updateItem($subId, $item, $expire = self::EXPIRE_NEVER) {
		
		$subId = $this::sanitizeId($subId);
		
		$this->pool[$subId] = CacheItem::create($subId, $expire, $item);
		
		$this->commitRequired = TRUE;
	}
	
	/**
	 * Removes an item from the pool.
	 *
	 * @param $subId
	 *   The id of the item.
	 */
	public function removeItem($subId) {
		
		$subId = $this::sanitizeId($subId);
		
		if (!array_key_exists($subId, $this->pool)) {
			return;
		}
		
		unset($this->pool[$subId]);
		
		$this->commitRequired = TRUE;
	}
	
	/**
	 * Commits all changes. Do not use it in Drupal. It is called once in hook_exit().
	 */
	public function commit() {
		
		if (! $this->commitRequired) {
			return;
		}
		
		$this->_updateCachePoolFile();
		$this->commitRequired = FALSE;
	}
	
	/**
	 * Flushes the entire pool.
	 */
	public function flush() {

		$this->pool = [];
		$this->_deleteCachePoolFile();
	}
	
	/**
	 * Checks whether the pool is empty.
	 */
	public function isEmpty() {
		
		return empty($this->pool);
	}
	
	/**
	 * Sanitizes an id.
	 *
	 * @param $unsanitizedId
	 *   The id to sanitize.
	 */
	public static function sanitizeId($unsanitizedId) {
		return implode('_', preg_split('/[\W]/', $unsanitizedId));
	}
	
	/**
	 * Removes all pools from disc.
	 */
	public static function clearAll() {

		$files = glob(drupal_realpath('private://') . DIRECTORY_SEPARATOR . 'LMACCache/*');
		foreach($files as $file){ 
			if(is_file($file)) {
				unlink($file);
			}
		}
	}
	
	/**
	 * Deletes a cache pool file.
	 */
	private function _deleteCachePoolFile() {

		unlink($this->cacheFileName);
	}
	
	/**
	 * Loads the entire cache pool from disc.
	 */
	private function _loadPoolFile() {
		
		if (! file_exists($this->cacheFileName)) {
			return;
		}
		
		$cacheFile = fopen($this->cacheFileName, "r");
		if (! $cacheFile) {
			throw new CannotLoadCacheFileException('Cache file \'' . $this->cacheFileName . '\' cannot be loaded.');
		}
		
		$cacheJson = '';
		while(!feof($cacheFile)) {
		  $cacheJson .= fgets($cacheFile);
		}
		fclose($cacheFile);
		
		$cacheFromJson = drupal_json_decode($cacheJson);
		$this->pool = [];		
		
		foreach($cacheFromJson as $cacheItem) {		
			if ($cacheItem['expire'] > self::EXPIRE_NEVER && $cacheItem['expire'] < time()) {
				continue;
			}
	 
			$this->pool[$cacheItem['subId']] = CacheItem::create($cacheItem['subId'], $cacheItem['expire'], $cacheItem['itemData']); 
		} 
	}
	
	/**
	 * Writes all changes to disc.
	 */
	private function _updateCachePoolFile() {
		
		$cacheFile = fopen($this->cacheFileName, "w");
		if (! $cacheFile) {
			throw new CannotCreateCacheFiledException('Cannot create cache file \'' . $this->cacheFileName . '\'.');
		}

		fwrite($cacheFile, drupal_json_encode($this->pool));
		fclose($cacheFile);
	}
}