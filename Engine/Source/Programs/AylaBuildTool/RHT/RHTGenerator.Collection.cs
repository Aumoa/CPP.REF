// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using static AylaEngine.RHTGenerator;
using System.Reflection.PortableExecutable;
using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public class Collection
    {
        private readonly Dictionary<string, List<RHTGenerator>> m_Group;

        public Collection(Dictionary<string, List<RHTGenerator>> group)
        {
            m_Group = group;
        }

        public bool FindMatch(TypeName typeName, AClass current, [NotNullWhen(true)] out RHTGenerator? generator, [NotNullWhen(true)] out AClass? @class)
        {
            if (m_Group.TryGetValue(typeName.Cpp, out var generators))
            {
                var pairs = generators.SelectMany(p => p.Classes, (a, b) => (a, b));

                // If a namespace is specified along with the class declaration, the explicit namespace is prioritized.
                if (typeName.Namespace != null)
                {
                    var match = pairs.FirstOrDefault(p => p.b.Class.Name == typeName.Cpp && p.b.Class.Namespace == typeName.Namespace);
                    if (match.a != null)
                    {
                        generator = match.a;
                        @class = match.b;
                        return true;
                    }
                }
                else
                {
                    // Otherwise, it is initially assumed to be the same as the current class's namespace.
                    var match = pairs.FirstOrDefault(p => p.b.Class.Name == typeName.Cpp && p.b.Class.Namespace == current.Class.Namespace);
                    if (match.a != null)
                    {
                        generator = match.a;
                        @class = match.b;
                        return true;
                    }

                    // Finally, it is assumed to be the engine namespace(Ayla).
                    match = pairs.FirstOrDefault(p => p.b.Class.Name == typeName.Cpp && p.b.Class.Namespace == "Ayla");
                    if (match.a != null)
                    {
                        generator = match.a;
                        @class = match.b;
                        return true;
                    }
                }
            }

            generator = null;
            @class = null;
            return false;
        }
    }
}
