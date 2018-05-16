using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class PathManager : MonoBehaviour
{
    public static PathManager instance;
    public int poolMaxSize = 20;

    public Transform pathDrawerPrefab;
    public Tilemap collisionTilemap;

    private PathDrawer[] m_pool;
    private int m_poolCursor = 0;

    private void Awake()
    {
        instance = this;
        m_pool = new PathDrawer[poolMaxSize];
        for (int i = 0; i < poolMaxSize; i++)
        {
            Transform drawerTransform = Instantiate<Transform>(pathDrawerPrefab);
            drawerTransform.SetParent(transform);
            m_pool[i] = drawerTransform.GetComponent<PathDrawer>();
            m_pool[i].collisionTilemap = collisionTilemap;
            drawerTransform.gameObject.SetActive(false);
            drawerTransform.transform.localPosition = Vector3.zero;
        }
    }


    public static PathDrawer CreatePathDrawer()
    {
        return instance.CreatePathDrawerImpl();
    }

    private PathDrawer CreatePathDrawerImpl()
    {
        if (m_poolCursor < m_pool.Length)
        {
            PathDrawer result = m_pool[m_poolCursor];
            m_poolCursor++;
            result.gameObject.SetActive(true);
            return result;
        }
        else return null;
    }

    public static void ReleasePathDrawer(PathDrawer drawer)
    {
        instance.ReleasePathDrawerImpl(drawer);
    }

    private void ReleasePathDrawerImpl(PathDrawer drawer)
    {
        if (m_poolCursor > 0)
        {
            m_poolCursor--;
            m_pool[m_poolCursor] = drawer;
        }
        drawer.gameObject.SetActive(false);

    }
}
