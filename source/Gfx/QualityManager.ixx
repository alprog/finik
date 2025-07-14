export module QualityManager;

import Singleton;
export import QualitySettings;

export class QualityManager : public Singleton<QualityManager>
{
public:
    const QualitySettings& getCurrent()
    {
        return settings;
    }

    void apply(QualitySettings settings);

protected:
    QualitySettings settings;
};
