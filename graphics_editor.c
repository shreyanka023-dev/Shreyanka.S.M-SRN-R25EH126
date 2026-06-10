#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 25
#define COLS 60
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef enum
{
    RECTANGLE,
    LINE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct
{
    int active;
    ShapeType type;

    int x1, y1;
    int x2, y2;

    int x3, y3;
    int radius;

} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

/* ---------- Canvas Functions ---------- */

void initCanvas()
{
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            canvas[i][j] = '_';
}

void displayCanvas()
{
    printf("\n");

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
            printf("%c", canvas[i][j]);

        printf("\n");
    }
}

void setPixel(int x, int y, char ch)
{
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
        canvas[y][x] = ch;
}

/* ---------- Drawing Algorithms ---------- */

void drawLinePixels(int x1, int y1, int x2, int y2, char ch)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (1)
    {
        setPixel(x1, y1, ch);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectanglePixels(int x1, int y1, int x2, int y2, char ch)
{
    for (int x = x1; x <= x2; x++)
    {
        setPixel(x, y1, ch);
        setPixel(x, y2, ch);
    }

    for (int y = y1; y <= y2; y++)
    {
        setPixel(x1, y, ch);
        setPixel(x2, y, ch);
    }
}

void drawCirclePixels(int xc, int yc, int r, char ch)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    while (x <= y)
    {
        setPixel(xc + x, yc + y, ch);
        setPixel(xc - x, yc + y, ch);
        setPixel(xc + x, yc - y, ch);
        setPixel(xc - x, yc - y, ch);

        setPixel(xc + y, yc + x, ch);
        setPixel(xc - y, yc + x, ch);
        setPixel(xc + y, yc - x, ch);
        setPixel(xc - y, yc - x, ch);

        if (d < 0)
            d += 2 * x + 3;
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }

        x++;
    }
}

void drawTrianglePixels(
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    char ch)
{
    drawLinePixels(x1, y1, x2, y2, ch);
    drawLinePixels(x2, y2, x3, y3, ch);
    drawLinePixels(x3, y3, x1, y1, ch);
}

/* ---------- Object Rendering ---------- */

void renderObject(Object obj, char ch)
{
    switch (obj.type)
    {
    case RECTANGLE:
        drawRectanglePixels(
            obj.x1, obj.y1,
            obj.x2, obj.y2, ch);
        break;

    case LINE:
        drawLinePixels(
            obj.x1, obj.y1,
            obj.x2, obj.y2, ch);
        break;

    case CIRCLE:
        drawCirclePixels(
            obj.x1, obj.y1,
            obj.radius, ch);
        break;

    case TRIANGLE:
        drawTrianglePixels(
            obj.x1, obj.y1,
            obj.x2, obj.y2,
            obj.x3, obj.y3,
            ch);
        break;
    }
}

void redrawCanvas()
{
    initCanvas();

    for (int i = 0; i < objectCount; i++)
    {
        if (objects[i].active)
            renderObject(objects[i], '*');
    }
}

/* ---------- Add Objects ---------- */

void addRectangle()
{
    if (objectCount >= MAX_OBJECTS)
    {
        printf("Maximum objects reached!\n");
        return;
    }

    Object obj;

    obj.active = 1;
    obj.type = RECTANGLE;

    printf("Top-left (x y): ");
    scanf("%d %d", &obj.x1, &obj.y1);

    printf("Bottom-right (x y): ");
    scanf("%d %d", &obj.x2, &obj.y2);

    objects[objectCount++] = obj;
    redrawCanvas();
}

void addLine()
{
    if (objectCount >= MAX_OBJECTS)
    {
        printf("Maximum objects reached!\n");
        return;
    }

    Object obj;

    obj.active = 1;
    obj.type = LINE;

    printf("Point1 (x y): ");
    scanf("%d %d", &obj.x1, &obj.y1);

    printf("Point2 (x y): ");
    scanf("%d %d", &obj.x2, &obj.y2);

    objects[objectCount++] = obj;
    redrawCanvas();
}

void addCircle()
{
    if (objectCount >= MAX_OBJECTS)
    {
        printf("Maximum objects reached!\n");
        return;
    }

    Object obj;

    obj.active = 1;
    obj.type = CIRCLE;

    printf("Center (x y): ");
    scanf("%d %d", &obj.x1, &obj.y1);

    printf("Radius: ");
    scanf("%d", &obj.radius);

    objects[objectCount++] = obj;
    redrawCanvas();
}

void addTriangle()
{
    if (objectCount >= MAX_OBJECTS)
    {
        printf("Maximum objects reached!\n");
        return;
    }

    Object obj;

    obj.active = 1;
    obj.type = TRIANGLE;

    printf("Vertex1 (x y): ");
    scanf("%d %d", &obj.x1, &obj.y1);

    printf("Vertex2 (x y): ");
    scanf("%d %d", &obj.x2, &obj.y2);

    printf("Vertex3 (x y): ");
    scanf("%d %d", &obj.x3, &obj.y3);

    objects[objectCount++] = obj;
    redrawCanvas();
}

/* ---------- Delete Object ---------- */

void deleteObject()
{
    int id;

    printf("Object ID (0-%d): ", objectCount - 1);
    scanf("%d", &id);

    if (id >= 0 && id < objectCount)
    {
        objects[id].active = 0;
        redrawCanvas();
        printf("Object deleted.\n");
    }
    else
    {
        printf("Invalid ID.\n");
    }
}

/* ---------- Modify Object ---------- */

void modifyObject()
{
    int id;

    printf("Object ID (0-%d): ", objectCount - 1);
    scanf("%d", &id);

    if (id < 0 || id >= objectCount)
    {
        printf("Invalid ID.\n");
        return;
    }

    Object *obj = &objects[id];

    switch (obj->type)
    {
    case RECTANGLE:
        printf("New top-left (x y): ");
        scanf("%d %d", &obj->x1, &obj->y1);

        printf("New bottom-right (x y): ");
        scanf("%d %d", &obj->x2, &obj->y2);
        break;

    case LINE:
        printf("New point1 (x y): ");
        scanf("%d %d", &obj->x1, &obj->y1);

        printf("New point2 (x y): ");
        scanf("%d %d", &obj->x2, &obj->y2);
        break;

    case CIRCLE:
        printf("New center (x y): ");
        scanf("%d %d", &obj->x1, &obj->y1);

        printf("New radius: ");
        scanf("%d", &obj->radius);
        break;

    case TRIANGLE:
        printf("Vertex1 (x y): ");
        scanf("%d %d", &obj->x1, &obj->y1);

        printf("Vertex2 (x y): ");
        scanf("%d %d", &obj->x2, &obj->y2);

        printf("Vertex3 (x y): ");
        scanf("%d %d", &obj->x3, &obj->y3);
        break;
    }

    redrawCanvas();
}

/* ---------- List Objects ---------- */

void listObjects()
{
    char *names[] = {
        "Rectangle",
        "Line",
        "Circle",
        "Triangle"};

    printf("\nObjects:\n");

    for (int i = 0; i < objectCount; i++)
    {
        if (objects[i].active)
            printf("ID %d : %s\n", i, names[objects[i].type]);
    }
}

/* ---------- Main ---------- */

int main()
{
    int choice;

    initCanvas();

    printf("=====================================\n");
    printf("      2D GRAPHICS EDITOR IN C\n");
    printf("      Shreya Uvaraj - R25EH125\n");
    printf("=====================================\n");

    while (1)
    {
        printf("\n===== MENU =====\n");
        printf("1. Add Rectangle\n");
        printf("2. Add Line\n");
        printf("3. Add Circle\n");
        printf("4. Add Triangle\n");
        printf("5. Delete Object\n");
        printf("6. Modify Object\n");
        printf("7. Display Picture\n");
        printf("8. List Objects\n");
        printf("0. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addRectangle();
            break;

        case 2:
            addLine();
            break;

        case 3:
            addCircle();
            break;

        case 4:
            addTriangle();
            break;

        case 5:
            deleteObject();
            break;

        case 6:
            modifyObject();
            break;

        case 7:
            displayCanvas();
            break;

        case 8:
            listObjects();
            break;

        case 0:
            printf("Exiting...\n");
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
