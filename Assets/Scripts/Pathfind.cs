using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class Pathfind : MonoBehaviour
{
    public static List<Cell> SimplePath(Cell A, Cell B)
    {
        List<Cell> result = new List<Cell>();
        for (int i = A.x; i < B.x; i++)
        {
            for (int j = A.y; j < B.y; j++)
            {
                result.Add(new Cell(i, j));
            }
            for (int j = A.y; j > B.y; j--)
            {
                result.Add(new Cell(i, j));
            }
        }
        for (int i = A.x; i > B.x; i--)
        {
            for (int j = A.y; j < B.y; j++)
            {
                result.Add(new Cell(i, j));
            }
            for (int j = A.y; j > B.y; j--)
            {
                result.Add(new Cell(i, j));
            }
        }
        result.Add(B);
        return result;
    }

    public class CellData
    {
        public Cell cell;
        public int length;

        public CellData(Cell inCell, int inLength) { cell = inCell; length = inLength; }
    }

    public static DataGrid<CellData> Dijkstra(Cell A, Tilemap collisionMap, int maxLength = 50)
    {
        DataGrid<CellData> result = new DataGrid<CellData>(A - new Cell(maxLength, maxLength), A + new Cell(maxLength, maxLength));
        Queue<CellData> nextCells = new Queue<CellData>();

        nextCells.Enqueue(new CellData(A, 0));
        result[A] = new CellData(A, 0);

        Cell[] directions = { Cell.right, Cell.up, Cell.down, Cell.left };

        while(nextCells.Count > 0)
        {
            CellData cellData = nextCells.Dequeue();
            if(cellData.length < maxLength)
            {
                foreach(Cell direction in directions)
                {
                    Cell cellToTest = cellData.cell + direction;
                    GameTile tile = collisionMap.GetTile<GameTile>(new Vector3Int(cellToTest.x, cellToTest.y, 0));
                    if (result.IsInbound(cellToTest) && tile == null || tile.collisionType == 0)
                    {
                        if (result[cellToTest] == null || result[cellToTest].length > cellData.length + 1)
                        {
                            if (result[cellToTest] != null)
                                print("To replace length " + result[cellToTest].length);
                            result[cellToTest] = new CellData(cellData.cell, cellData.length + 1);
                            nextCells.Enqueue(new CellData(cellToTest, cellData.length + 1));
                        }
                    }
                }
            }
        }

        return result;
    }

    public static List<Cell> GetPathFromDijkstra(DataGrid<CellData> data, Cell A, Cell B)
    {
        List<Cell> result = new List<Cell>();
        result.Add(B);
        Cell currentCell = B;
        while(currentCell != A)
        {
            if (data[currentCell] == null)
                currentCell = A;
            else currentCell = data[currentCell].cell;
            result.Insert(0, currentCell);
        }
        return result;
    }
}

public class DataGrid<T>
{
    private T[] data;
    private Cell min, max;
    public Cell Min { get { return min; } }
    public Cell Max { get { return max; } }

    public bool IsInbound(Cell cell)
    {
        return cell.x >= min.x && cell.y >= min.y && cell.x <= max.x && cell.y <= max.y;
    }

    public DataGrid(Cell inMin, Cell inMax)
    {
        min = inMin;
        max = inMax;
        data = new T[size.x * size.y];
        for(int i=0; i<size.x * size.y; i++)
        {
            data[i] = default(T);
        }
    }

    public Cell size { get { return max - min + new Cell(1, 1); } }

    public T this[Cell cell]
    {
        get
        {
            if(IsInbound(cell))
                return data[(cell - min).x + (cell - min).y * size.x];
            return default(T);
        }

        set
        {
            if (IsInbound(cell))
                data[(cell - min).x + (cell - min).y * size.x] = value;
        }
    }

}
