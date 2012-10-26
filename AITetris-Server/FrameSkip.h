#pragma once
// from http://skmagic.tistory.com/entry/FrameTimer%ED%81%B4%EB%9E%98%EC%8A%A4-%EB%A7%8C%EB%93%A4%EA%B8%B0
class FrameSkip 
{ 
public:
    // ��������� ����/�Ҹ���. 
    FrameSkip()
    { 
     Clear(); 
     } 
    virtual ~FrameSkip() { } 

protected: 
    float       m_Timer; 
    float       m_SecPerFrame;  // �����Ӵ� �ð�
 
public: 
    void Clear() 
    { 
        SetFramePerSec( 60.0f ); 
        m_Timer = 0.0f; 
    } 

    void SetFramePerSec( float fps ) 
    { 
        m_SecPerFrame = 1.0f/fps;  //fps �ݴ� spf
    } 
    
    // ���ϴ� �����Ӻ��� �ʹ� ������, 
    // false�� ��ȯ�ؼ� �ڵ带 ���۽�Ű�� �ʵ��� �Ѵ�. 
    // dt�� '��'���� (�и��� �ƴ�!!!) 
    bool Update( float dt ) 
    { 
        m_Timer += dt;       
 
        if( m_Timer<0 ) return false;       
 
        // �������ӿ� �ش��ϴ� �ð��� ����. 
        m_Timer -= m_SecPerFrame; 
 
        return true; 
    } 
    
    // Update�Ŀ� ȣ���ؼ� frame skip�� �����ؾ� �ϴ��� �˻��Ѵ�. 
    // frame skip�� �Ͼ�� �Ѵٸ� true�� ��ȯ�Ѵ�. 
    bool IsFrameSkip() 
    { 
        return m_Timer >= 0; 
    } 
    
}; 