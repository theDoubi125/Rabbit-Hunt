using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridTest : MonoBehaviour
{
    public Transform cursor;
    public Transform PrefabToSpawn;

	void Start ()
    {
		
	}
	
	void Update ()
    {
        Cell targetCell = GridManager.GetMouseCell();
        Vector2 targetCellCenter = GridManager.GetCellPos(targetCell);
        if (cursor != null)
        {
            cursor.transform.position = targetCellCenter;

        }

        if(Input.GetButtonDown("Fire1") && PrefabToSpawn != null)
        {
            Transform instance = Instantiate<Transform>(PrefabToSpawn, targetCellCenter, Quaternion.identity);
            GridManager.RegisterEntity(instance);
        }
	}
}
