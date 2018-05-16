using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public struct Cell
{
    public int x, y;

    public Cell(int x, int y)
    {
        this.x = x;
        this.y = y;
    }

    public static Cell right = new Cell(1, 0);
    public static Cell left = new Cell(-1, 0);
    public static Cell down = new Cell(0, -1);
    public static Cell up = new Cell(0, 1);

    public Transform GetEntities()
    {
        return GridManager.GetEntitiesInCell(this);
    }

    public static bool operator==(Cell A, Cell B)
    {
        return A.x == B.x && A.y == B.y;
    }

    public static bool operator!=(Cell A, Cell B)
    {
        return A.x != B.x || A.y != B.y;
    }

    public static Cell operator +(Cell A, Cell B)
    {
        return new Cell(A.x + B.x, A.y + B.y);
    }

    public static Cell operator -(Cell A, Cell B)
    {
        return new Cell(A.x - B.x, A.y - B.y);
    }
}

[CreateAssetMenu(fileName = "Grid Config", menuName = "Grid/GridConfig", order = 1)]
public class GridConfig : ScriptableObject
{
    public Vector2 cellSize;
}

public class GridManager : MonoBehaviour
{
    private static GridManager m_instance;
    public GridConfig gridConfig;
    private Dictionary<Cell, Transform> entities = new Dictionary<Cell, Transform>();

    private static GridConfig activeConfig { get { return m_instance.gridConfig; } }

    void Awake()
    {
        if(m_instance != null)
        {
            Destroy(m_instance);
        }
        m_instance = this;
        DontDestroyOnLoad(this);
    }

    public static Cell GetCellAt(Vector2 pos)
    {
        return new Cell(Mathf.RoundToInt(pos.x / activeConfig.cellSize.x), Mathf.RoundToInt(pos.y / activeConfig.cellSize.y));
    }

    public static Vector2 GetCellPos(Cell cell)
    {
        return new Vector2(cell.x * activeConfig.cellSize.x, cell.y * activeConfig.cellSize.y);
    }

    public static void RegisterEntity(Transform entity)
    {
        Cell cell = GetCellAt(entity.transform.position);
        if(!m_instance.entities.ContainsKey(cell))
        {
            GameObject node = new GameObject();
            node.transform.position = GetCellPos(cell);
            node.name = "Cell " + cell.x + ", " + cell.y;
            m_instance.entities.Add(cell, node.transform);
        }
        entity.SetParent(m_instance.entities[cell], true);
    }

    public static void UpdateEntityCell(Transform entity)
    {
        Cell cell = GetCellAt(entity.transform.position);
        if(!m_instance.entities.ContainsKey(cell))
        {
            GameObject node = new GameObject();
            node.transform.position = GetCellPos(cell);
            node.name = "Cell " + cell.x + ", " + cell.y;
            m_instance.entities.Add(cell, node.transform);
        }
        entity.SetParent(m_instance.entities[cell], true);
    }

    public static T GetEntityAt<T>(Cell cell)
    {
        if(m_instance.entities.ContainsKey(cell))
            return m_instance.entities[cell].GetComponentInChildren<T>();
        return default(T);
    }

    public static T[] GetEntitiesAt<T>(Cell cell)
    {
        return m_instance.entities[cell].GetComponentsInChildren<T>();
    }

    public static Transform GetEntitiesInCell(Cell cell)
    {
        if (m_instance.entities.ContainsKey(cell))
            return m_instance.entities[cell];
        return null;
    }

    public static Transform InstantiateTileMap(Transform tilemapPrefab)
    {
        return Instantiate<Transform>(tilemapPrefab, m_instance.transform);
    }

    public static Cell GetMouseCell()
    {
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        Vector2 target = ray.origin;
        return GridManager.GetCellAt(target);
    }
};
