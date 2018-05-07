using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class PlanningController : MonoBehaviour
{
    public List<Texture2D

    private bool selection = false;
    private Cell startCell, targetCell;

    private Tilemap tilemap;

    void Start()
    {
        tilemap = GetComponent<Tilemap>();
    }
	
	void Update ()
    {
        if (Input.GetButtonDown("Fire1"))
        {
            if (!selection)
                startCell = GridManager.GetMouseCell();
            selection = !selection;
        }
        if (targetCell != GridManager.GetMouseCell())
        {

        }
	}
}
