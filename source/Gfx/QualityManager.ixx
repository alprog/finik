export module QualityManager;

import Finik.Core.Singleton;
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
