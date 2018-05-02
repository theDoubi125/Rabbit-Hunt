using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestComponent : MonoBehaviour {
    
	void Start () {
		
	}
	
	void Update ()
    {
        GridManager.UpdateEntityCell(transform);
	}
}
