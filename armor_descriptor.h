#ifndef ARMOR_DESCRIPTOR_H
#define ARMOR_DESCRIPTOR_H
class ArmorDescriptor
{
public:
    ArmorDescriptor()
    {
    rotationScore = 0;
    sizeScore = 0;
    vertex.resize(4);
    for(int i = 0; i < 4; i++)
        {
        vertex[i] = cv::Point2f(0, 0);
        }
    }
    void clear()
    {
        rotationScore = 0;
        sizeScore = 0;
        distScore = 0;
        finalScore = 0;
        for(int i = 0; i < 4; i++)
        {
            vertex[i] = cv::Point2f(0, 0);
        }
    }
public:
    float sizeScore;		//S1 = e^(size)
    float distScore;		//S2 = e^(-offset)
    float rotationScore;		//S3 = -(ratio^2 + yDiff^2)
    float finalScore;

    std::vector<cv::Point2f> vertex; //four vertex of armor area, lihgt bar area exclued!!
        cv::Mat frontImg; //front img after prespective transformation from vertex,1 channel gray img
    //	0 -> small    1 -> big    -1 -> unkown
    int type;
};
#endif // ARMOR_DESCRIPTOR_H
